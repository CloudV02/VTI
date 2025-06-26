#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <termios.h>
#include <linux/i2c-dev.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <mosquitto.h>
#include <gpiod.h>

// --------- MQTT ---------
#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_LINE 17
#define ACCESS_TOKEN "nZPvxtnOANwrOVCcZVpk"

// --------- SPI ---------
#define SPI_DEVICE "/dev/spidev0.1"
uint8_t spi_mode = SPI_MODE_3;
uint8_t spi_bits = 8;
uint32_t spi_speed = 100000;

// --------- Globals for MQTT ---------
struct gpiod_chip *chip;
struct gpiod_line *line;
int latest_uart = 0, latest_i2c = 0, latest_spi = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void send_telemetry(struct mosquitto *mosq) {
    char msg[128];
    pthread_mutex_lock(&lock);
    snprintf(msg, sizeof(msg), "{\"uart\":%d,\"i2c\":%d,\"spi\":%d}", latest_uart, latest_i2c, latest_spi);
    pthread_mutex_unlock(&lock);
    mosquitto_publish(mosq, NULL, "v1/devices/me/telemetry", strlen(msg), msg, 1, false);
    printf("[MQTT] Sent: %s\n", msg);
}

// ---------- UART Thread ----------
void* uart_thread(void* arg) {
    int uart_fd = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart_fd == -1) {
        perror("UART open");
        return NULL;
    }

    struct termios options;
    tcgetattr(uart_fd, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_fd, TCIFLUSH);
    tcsetattr(uart_fd, TCSANOW, &options);

    char ch;
    while (1) {
        if (read(uart_fd, &ch, 1) == 1) {
            pthread_mutex_lock(&lock);
            latest_uart = (int)(uint8_t)ch;
            pthread_mutex_unlock(&lock);
            printf("[UART] Received: %d\n", latest_uart);
        }
        usleep(1000000);
    }

    close(uart_fd);
    return NULL;
}

// ---------- I2C Thread ----------
void* i2c_thread(void* arg) {
    const char *bus = "/dev/i2c-1";
    int addr = 0x08;
    uint8_t buffer[4];

    int file = open(bus, O_RDWR);
    if (file < 0) {
        perror("I2C open");
        return NULL;
    }
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        perror("I2C ioctl");
        close(file);
        return NULL;
    }

    while (1) {
        if (read(file, buffer, 1) == 1) {
            pthread_mutex_lock(&lock);
            latest_i2c = buffer[0];
            pthread_mutex_unlock(&lock);
            printf("[I2C] Received: %d\n", buffer[0]);
        } else {
            perror("[I2C] Read failed");
        }
        usleep(1000000);
    }

    close(file);
    return NULL;
}

// ---------- SPI Thread ----------
void* spi_thread(void* arg) {
    int spi_fd = open(SPI_DEVICE, O_RDWR);
    if (spi_fd < 0) {
        perror("SPI open");
        return NULL;
    }

    ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode);
    ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits);
    ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);

    while (1) {
        uint8_t tx = 0xFF;
        uint8_t rx;

        struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long)&tx,
            .rx_buf = (unsigned long)&rx,
            .len = 1,
            .delay_usecs = 0,
            .speed_hz = spi_speed,
            .bits_per_word = spi_bits,
        };
        int ret = ioctl(spi_sd, SPI_IOC_MASSAGE(1),&tr);

        if (ret !=1) {
            perror("SPI transfer failed");
        } else{
            pthread_mutex_lock(&lock);
            latest_spi = rx;
            pthread_mutex_unlock(&lock);
            printf("[SPI] Received: %d\n", rx);
}

        usleep(1000000);
    }

    close(spi_fd);
    return NULL;
}

// ---------- GPIO ----------
void gpio_init() {
    chip = gpiod_chip_open(GPIO_CHIP);
    line = gpiod_chip_get_line(chip, GPIO_LINE);
    gpiod_line_request_output(line, "thingsboard", 0);
}

void gpio_set(struct mosquitto *mosq, int value) {
    gpiod_line_set_value(line, value);
    printf("[MQTT] LED %s\n", value ? "ON" : "OFF");

    char payload[64];
    snprintf(payload, sizeof(payload), "{\"led\": %s}", value ? "true" : "false");
    mosquitto_publish(mosq, NULL, "v1/devices/me/telemetry", strlen(payload), payload, 1, false);
}

// ---------- MQTT Callback ----------
void on_connect(struct mosquitto *mosq, void *obj, int rc) {
    if (rc == 0) {
        printf("[MQTT] Connected.\n");
        mosquitto_subscribe(mosq, NULL, "v1/devices/me/rpc/request/+", 0);
    } else {
        printf("[MQTT] Connect failed: %d\n", rc);
    }
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
    printf("[MQTT] Payload: %s\n", (char *)msg->payload);

    if (strstr(msg->payload, "\"method\":\"setValue\"") && strstr(msg->payload, "true")) {
        gpio_set(mosq, 1);
    } else if (strstr(msg->payload, "\"method\":\"setValue\"") && strstr(msg->payload, "false")) {
        gpio_set(mosq, 0);
    }
}

// ---------- MQTT Thread ----------
void* mqtt_thread(void* arg) {
    gpio_init();
    mosquitto_lib_init();
    struct mosquitto *mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        perror("[MQTT] Instance failed");
        return NULL;
    }

    mosquitto_username_pw_set(mosq, ACCESS_TOKEN, NULL);
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    if (mosquitto_connect(mosq, "demo.thingsboard.io", 1883, 60)) {
        perror("[MQTT] Connect error");
        return NULL;
    }

    while (1) {
        mosquitto_loop(mosq, -1, 1);
        send_telemetry(mosq);
        sleep(1);
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    gpiod_chip_close(chip);
    return NULL;
}

// ---------- Main ----------
int main() {
    pthread_t uart_tid, i2c_tid, spi_tid, mqtt_tid;

    pthread_create(&uart_tid, NULL, uart_thread, NULL);
    pthread_create(&i2c_tid, NULL, i2c_thread, NULL);
    pthread_create(&spi_tid, NULL, spi_thread, NULL);
    pthread_create(&mqtt_tid, NULL, mqtt_thread, NULL);

    pthread_join(uart_tid, NULL);
    pthread_join(i2c_tid, NULL);
    pthread_join(spi_tid, NULL);
    pthread_join(mqtt_tid, NULL);

    return 0;
}