# GPIO

## Một số các define trong thư viện của GPIO

Giờ ta sẽ đi phân tích các define đó:
- Các GPIO: như GPIOA, GPIOB, GPIOC, ... sẽ được định nghĩa như nào. Thì để các GPIOA có thể trỏ tới như GPIOA->CRL thì GPIOA này cần phải là struct.

- Các thuộc tính của GPIO sẽ được lưu vào trong 1 GPIO_InitTypeDef gồm có chọn Pin, có mode của Pin đấy là in hay out kiểu vậy và Speed của Pin.
+ Ta phải define cả Pin cho nó trừu tượng như GPIO_Pin_10  0x0400
+ Mode và Speed sẽ có 1 cái enum để trừ tượng tên gọi

- Đó thì ta thấy cái luồng để tạo define như sau đầu tiên ta thấy ta cần 1 cái trừu tượng như GPIOA trỏ tới CRL chẳng hạn thì như phần code ví dụ 1 bên dưới ta đã trình bày rõ ràng. Nhưng nhìn tổng quan là ta sẽ muốn 1 cái hàm hay cái struct dùng để mình điền cài Pin gì vào thì thiết lập cái pin đó thì dùng GPIO_InitTypedef struct. Kiểu như struct này sẽ chứa việc mình thiết lập Pin nào dùng trong hệ thống, còn cách dùng ra sao thì dựa vào hàm khác như GPIO_SetPin chẳng hạn và ở trong đó mình trỏ GPIOA->ODR chẳng hạn.


## GPIO Mode:

- GPIO_Mode_AIN: Analog Input, chân được cấu hình làm đầu vào là analog -> chân ấy sẽ sử dụng cho ADC.

- GPIO_Mode_IN_FLOATING: Floating Input, trạng thái thả nổi tức là chân được cấu hình là đầu vào, nhưng nó không được thiết lập cố định ở mức cao hay mức thấp, mà nó ở giữa giữa.

- GPIO_Mode_IPD: Input with Pull-down, chân GPIO được cấu hình là chân vào và nó sẽ có 1 điện trở nội bộ kéo xuống GND được kích hoạt. Khi không có tín hiệu nào được áp dụng lên chân này thì nó sẽ ở mức thấp / mức 0.

- GPIO_Mode_IPU: Input with Pull-up, chân GPIO được cấu hình là đầu vào và được nối với điện trở kéo lên. Khi không có tín hiệu nào được áp dụng lên chân này, nó sẽ được kéo lên mức cao / mức 1.

Note muốn biết điện áp đo dòng điện kiểu gì khi có trở kháng cao thì xem nguyên lý hoạt động của vôn kế. Và thêm nữa, dòng điện phải chạy nếu có mạch kín, còn điện áp thì nó vẫn sẽ ở nguyên đó, giống như điện ở trong nhà vẫn luôn có kể cả mình không dùng.

- GPIO_Mode_Out_OD: Open-drain Output, ở chế độ này chân được phép kéo xuống mức thấp bằng phần mềm, còn muốn kéo lên ở mức high thì nối điện trở kéo lên ngoài. Tức là ở trong mạch sẽ không có điện trở kéo lên chỉ có kéo xuống -> phải lắp ngoài. 

- GPIO_Mode_Out_PP: Push-pull Output, thì chân này là đầu ra và mình có thể thiết lập ở cả mức cao và thấp mà không cần phần cứng nào.

- GPIO_Mode_AF_OD: Alternate Function Open-drain, chân GPIO sẽ được cấu hình để hoạt động trong 1 số chức năng thay thế như UART, I2C,... và sử dụng chế độ open-drain -> chỉ sử dụng như output.

- GPIO_Mode_AF_PP: Alternate Function Push-Pull, chân GPIO sẽ được cấu hình để sử dụng các chức năng thay thế như UART,I2C và sử dụng chế độ push-pull.

Ví dụ 1 như sau: Như ta thấy cái địa chỉ của GPIOA_BASE đã được ép kiểu thành 1 con trỏ struct GPIO_Typedf. Vậy nên mỗi lần gọi GPIOA nghĩa là gọi đến cái địa chỉ GPIOA_BASE và nó là struct nên có thể trỏ vào từng member. Ngoài ra __IO là 1 định nghĩa trong thư viên CMSIS, nó có nghĩa là volitile.
```C
#define GPIO_Pin_0          ((uint16_t)0x0001)
#define GPIO_Pin_1          ((uint16_t)0x0002)
#define GPIO_Pin_2          ((uint16_t)0x0004)
#define GPIO_Pin_3          ((uint16_t)0x0008)
#define GPIO_Pin_4          ((uint16_t)0x0010)
#define GPIO_Pin_5          ((uint16_t)0x0020)
#define GPIO_Pin_6          ((uint16_t)0x0040)
#define GPIO_Pin_7          ((uint16_t)0x0080)
#define GPIO_Pin_8          ((uint16_t)0x0100)
#define GPIO_Pin_9          ((uint16_t)0x0200)
#define GPIO_Pin_10         ((uint16_t)0x0400)
#define GPIO_Pin_11         ((uint16_t)0x0800)
#define GPIO_Pin_12         ((uint16_t)0x1000)
#define GPIO_Pin_13         ((uint16_t)0x2000)
#define GPIO_Pin_14         ((uint16_t)0x4000)
#define GPIO_Pin_15         ((uint16_t)0x8000)
typedef struct{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
}GPIO_Typedef; //Typedef thanh ghi
#define GPIOA_BASE (0x40000545UL)
#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)

typedef enum{
    GPIO_Speed_10Mhz = 0x1;
    GPIO_Speed_2Mhz  = 0x2;
    GPIO_Speed_50Mhz = 0x3;
}GPIOSpeed_Typedef;

typedef enum{
    GPIO_Mode_AIN = 0x00;
    GPIO_Mode_IN_FLOATING = 0x04;
    GPIO_Mode_IPD = 0x28; // đáng lẽ là 0x08 là đủ input rồi nhưng mà thêm 2 và 4 như bên dưới vì trong GPIO_Init mà CMSIS cung cấp cho ta ấy thì nó sẽ dựa vào bit 0x28 và 0x48 để set ODR tương ứng trog cái hàm GPIO_Init.
    GPIO_Mode_IPU = 0x48;
    GPIO_Mode_Out_OD = 0x14;
    GPIO_Mode_PP = 0x10;
    GPIO_Mode_AF_OD = 0x1C;
    GPIO_Mode_AF_PP = 0x18;
}GPIOMode_Typedef;

typedef struct{
    uint16_t GPIO_Pin;
    GPIOSpeed_Typedef   GPIO_Speed;
    GPIOMode_Typedef    GPIO_Mode;
}GPIO_InitTypedef; //Typedef hoat dong

int main(){

}

```


# Ngắt với Timer

## Ngắt
thì nói về ngắt thì sẽ có trình phục vụ ngắt riêng trong vi điều khiển ví như EXIT1 là 1 hàm phục vụ ngắt chẳng hạn có thể được tìm thấy được khi trong bảng Vector table. Các ngắt thì sẽ có địa chỉ cố định và mình được phép đặt tên và đki vị trí của ngắt thông qua bảng Vector table.

- Ngắt ngoài: là ngắt xảy ra bởi ngoại vi khi có sự thay đổi điện áp ở các PIN, các chân GPIO như LOW là ngắt xảy ra khi chân ở mức thấp, HIGH ngắt xảy ra khi chân ở mức cao, Rising là ngắt xảy ra khi trạng thái chuyển từ thấp lên cao, Falling là ngắt xảy ra khi trạng thái chân chuyển từ mức cao xuống thấp.

- Ngắt Exception: Ngắt nội là các ngắt của hệ thống như HardFauld, BusFauld kiểu như trong lúc chạy nó sẽ sinh ra vậy.

- Ngắt timer: Ngắt timer xảy ra khi giá trị đếm kiểu bị tràn hay có thể nói nó chạm tới cái giá trị mình thiết lập được xác định bởi 1 thanh ghi đếm của timer. Và đây là ngắt nội trong MCU nên mình phải reset giá trị thanh ghi không nó sẽ tiếp tục đếm tràn -> phải reset để nó chỉ đếm đến giá trị mình mong muốn lúc đó mới tạo ra được ngắt tiếp theo.

- Ngắt truyền nhận: Xảy ra khi có sự kiện truyền nhận dữ liệu thoi, ngắt này thường xảy ra để đảm bảo quá trình truyền nhận xảy ra chính xác. Ví dụ như 1 MCU đang truyền data mà bên MCU khác lại đang đếm timer thì 2 MCU đang không hiểu nhau, thì đơn gian giờ MCU A bắt đầu truyền data nó sẽ gửi 1 tín hiệu ngắt cho MCU B và MCU B sẽ bắt đầu quá trình nhận.

- Ngoài ra ngắt còn có lý thuyết nữa đó là độ ưu tiên ngắt, và độ ưu tiên ngắt này sẽ được thực hiện bởi NVIC, ngắt có độ ưu tiên càng thấp thì quyền càng cao.

## Timer
nói đơn giản thì timer là 1 bộ đếm (đếm lên hoặc xuống) bên trong Timer, ngoài ra nó có thể ở chế độ counter, tức là mình sẽ tác động ngoại vi vào như mỗi lần nhấn nút thì CNT sẽ tăng lên 1 chẳng hạn. Thì các TIMx_CH chính là các chân đầu vào đó. Và 1 Timer chỉ sử dụng được 1 chế độ, 1 là đếm nội là đếm kiểu liên tục í làm delay các thứ, 2 là như chế độ counter đếm dựa vào ngoại vi. Việc lựa chọn clock nội với xung ngoài sẽ dựa vào thanh ghi SMCR thì như ta thấy trong tài liệu thì value reset là 0x0000 tức là nó sẽ auto sử dụng clock nội.

**Giờ ta sẽ nói về 1 số vấn đề cần tìm hiểu trong TIMER**
- Prescaler: là bộ chia tần số. Thì nó để làm gì? Thì nói đơn giản như cái tên của nó là dùng để chia tần số. Ví dụ như trong Timer thì tần số nó được cấp mặc định dựa vào file system.c của hệ thống, thì nó được cấp 72Mhz. Giờ mà ta để nguyên 72Mhz và cho nó đếm thì nó sẽ đếm tràn thanh ghi rất nhanh. Nói thêm về đếm tràn tức là cái thanh ghi giữ giá trị đếm sẽ bị tối đa. Ví dụ như ở TIM2 là 16bit và sẽ có dải đếm 0->65535 và nó đếm quá 65535 là bị tràn và phải quay về 0. Vậy tức là nếu để nguyên 72Mhz tức là thời gian nó đếm 1 tick là đếm từ 0 lên 1 hay từ 1 lên 2 trong thanh ghi đếm của TIM kia sẽ là 1/72.000.000hz, thì việc đếm quá nhanh khiến ta không thu được số liệu gì vì ví dụ nó đếm xong reset, thì mình không biết nó đếm được bao nhiêu. Giờ ta sẽ đưa ra ví dụ giống như đồng hồ đếm từ 0->59 xong lại reset, nó sẽ giống với chạy từ 0->65535, còn 0->1 đối với đồng hồ là nó sẽ được thiết lập là 1s, còn đối với vi điều khiển như mình nói bên trên sẽ là 1/72.000.000. Thì nếu ta thiết lập tần số quá nhanh thì cái dải từ 0->65535 nó cũm bị tràn nhanh, kiểu các con số trong đấy nó lấy ra không có nhiều ý nghĩa. Vậy nên ta cần phải kéo giảm tần số lại để giảm thời gian, nó tràn. Thì ví dụ như cái đồng hồ kia từ 0->1 là ta biết đó là 1s. Thì giờ trong vi điều khiển ta sẽ cố gắng thiết lặp sao cho 0->1 sẽ là 1ms chẳng hạn. Và từ đó bộ chia tần ra đời để làm thay đổi cái tần số thay vì từ 0->1 chạy trong 0.00000072 thì giờ thành 0.0001 thoi, chậm hơn sẽ ứng dụng thực tế nhiều hơn.

- Time Clock: hay thanh ghi CNT, nó sẽ dựa vào mỗi xung hay mỗi tick của clock để đếm lên 1, nó giống với cái 0->65535 t ví dụ ở trên đóa.

- Auto Reload Value: hay ARR là giá trị tối đa mà timer sẽ đếm trước đó, nếu vượt quá giá trị mình set cho ARR nó sẽ quay về 0. Và CNT là thanh ghi sẽ quay về 0.

Tổng kết: Thì Prescaler(bộ chia tần) sẽ quyết định tốc độ nhảy từ 0->1 của giá trị trong thanh ghi CNT, còn ARR sẽ quyết định giá trị tràn của CNT, ví dụ như ARR = 1000, thì CNT sẽ đếm đến 1000 rồi quay về 0, với tốc độ = 1000*(thời gian mà nó nhảy lên 1 tick).


- Capture trong Timer là cái gì? 



# MPU (Memory Protection Unit)

Tại phần này mình sẽ lâpj trình về thanh ghi MPU nên mình sẽ tập trung nói chủ yếu về thanh ghi, cách hoạt động của các thanh ghi

## 1.Lý thuyết
Thì MPU như cái tên là nó sẽ bảo vệ vùng nhớ hoặc sở hữu hay làm bất cứ cái gì nó thích đối với các vùng nhớ nó sở hữu. Thì theo ta được biết nó sẽ có 8 region ở cortex m4, các region có thể ghi đè lên nhau và độ ưu tiên sẽ vào region cao nhất region 7, tức là càng region càng bé nếu đè lên thì những cái vùng ghi chung đó sẽ lấy thuộc tính của region cao hơn VD: region 1 và region 4 mà ghi chung vùng nhớ 0x20100 - 0x20110 chẳng hạn, thì cái vùng nhớ đó sẽ sở hữu các đặc điểm của region 4

Cách để tìm lỗi thì khi xảy ra lỗi của MPU nó sẽ nhảy vào MemManage Exception hoặc Hardfauld và lỗi sẽ được thông báo vào thanh ghi MMFSR - MemManage Fault Status Register và nơi sẽ lưu địa gây ra lỗi là MMFAR - MemManage Fault Address Register.

## 2. Thanh ghi

**Thanh ghi MPU_CTRL**
- Bit 2: PRIVDEFENA bit này sẽ cho phép truy cập vào default memory map ở chế độ privileged. Thì nói thêm thì dèault memory map là vùng không được cấp hình bởi MPU, tức là các vùng không nằm trong region nào. Và ở chế độ privileged là sao? Nếu để bit này là 0 thì sẽ không được phép truy cập luôn, tức là kể cả privileged hay unprivileged cũm không được. Còn nếu set lên 1 tức là nếu mình đang ở chế độ privileged sẽ được phép truy cập thỏa mái.

- Bit 1: HFNMIENA bit này cho phép MPU hoạt động trong Hardfault và Memfault. Tức là sao ? Giờ ví dụ Hardfault hay Memfault nó nằm trong vùng quản lý của MPU là không được truy cập đến, tức là không đọc được, hoặc là nó không thuộc region nào của MPU và mình cũm không bật bit PRIVDEFENA lên -> cả 2 trường hợp đó xảy ra thì mình sẽ không bao h vào được hàm Hardfault hay Memfault -> khi có 1 lỗi xảy ra, nó sẽ tìm tới 2 cái hàm này để trả về kết quả fault, nhưng mà mình khóa con mịa nó vào bằng MPU rồi :)) thì giờ nó cố truy cập tới hàm đó, thì nó lại lỗi và nhảy vào hàm lỗi tiếp -> bị lồng lỗi. Vâyj nên bật bit này lên nó sẽ bỏ quả việc kiểm soát 2 hàm Fault này (Hardfault, Memfault).

- Bit 0: Enable bit này chỉ đơn giản là bật MPU.

**Thanh ghi MPU_RNR**: MPU Region Number Register
- REGION: bit 0-7 là nơi chọn region, theo cortex m3,m4 có 8 hoặc 16 vùng, và mình dùng vùng nào thì ghi giá trị tương ứng, và 1 thời điểm chỉ có 1 vùng. Ví dụ chọn region 3 thì ghi số 3 vào -> REGION ở cortex sẽ chỉ được ghi từ 0->7, còn ghi từ 8->255 nó sẽ bị lỗi.

**Thanh ghi MPU_RBAR**: Region Base Address Register

- REGION - bit [3:0]: Chọn vùng nhớ thiết lập giống với thanh ghi MPU_RNR. Và nó sẽ có bit VALID cũm nằm trong thanh ghi này để mình chọn sử dụng region của cái thanh ghi này hay của thanh ghi MPU_RNR.

- Bit 4 - VALID: với bit = 0 xác định region bằng thanh ghi MPU_RNR, bit = 1 xác định region ở thanh ghi này luôn.

- Bit[31:N] - ADDR: Chứa Base Address của Region, tức là địa chỉ bắt đầu của region. Thì nói về N trước là nó sẽ dựa vào SIZE ở cái thanh MPU_RASR bên dưới. Tức là nếu ta để SIZE là 32byte tương đương với để giá trị trong thanh ghi SIZE là 5 (thực tế nó sẽ là 4 nhưng theo công thức sẽ là 4+1 và ta sẽ lấy 2^(4+1) = 32Byte, đọc tài liệu core để hiểu). Và N sẽ được xác định bằng 32log2 = 5 -> N = 5. Câu hỏi đặt ra là địa chỉ nó có giá trị 32bit cơ mà sao chỉ từ 5->31 sao đủ 32bit lưu địa chỉ? Thì ví dụ ở đây N = 8 đi thì tức là nó sẽ từ bit 8->31, tương đương với việc SIZE sẽ là 2^8 = 256Byte, và giờ theo quy tắc là cái địa chỉ base nó phải chia hết cho 256Byte. Và hay cái :)) những cái chia hết cho 256Byte lại có 8 bit đầu tức là bit 0->7 sẽ bằng 0 hết :)) vậy nghĩa là thực tế mình chỉ cần quan tâm đến bit 8->31 xem có những bit nào là 1, còn các bit dưới 8 đều bằng 0 hết. Vậy tức là đối với N bằng bao nhiêu thì các bit từ 0->N-1 sẽ là bit 0. Còn nếu mà mình set các thanh ghi REGION hay VALID trước đó, thì nếu mình muốn lấy địa chỉ thì mình mask lại thui, kiểu t biến để mask lại, hoặc tạo 1 cái define gì đóa.

**Thanh ghi MPU_RASR**: MPU Region Attribute and Size Register.
Thanh ghi này là thanh ghi cấu hình thoi thif nó sẽ có:

- BIT 28 - XN: Bit này có nhiệm vụ là set cho vùng nhớ đó không được phép truy cập nếu mình set lên 1.

- Bit [26-24] - AP: các bit thanh ghi này sẽ cho biết quyền truy cập vào region. À thì vào đọc bảng chứ ghi ra không hết, tại cũm dễ hiểu.

- Bit [21:19,17,16] -TEX,C,B: nói chung là các bit này kiểu setup các thuộc tính nên đọc thẳng luôn trong tài liệu

- Bit 18 - S: Shareable bit thì nó cho phép chia sẻ vùng nhớ trong ứng dụn Multicore. Thì ncl ít dùng :))

- Bit [5:1] - SIZE: Kích thước của Region, thì nói chung bit này quan trọng, thì trước tiên ta sẽ nói về công thức tính size thực tế dựa trên trường này: Region = 2^(SIZE+1) , SIZE ở đây là mình chọn trong cái trường SIZE đó thì thấp nhất là chọn 4, cao nhất là 31.

- Bit 1 -ENABLE : để bật region thoi.

- bit 8-15 -SRD: Subregion tức là cái vùng nhớ mình cấp í sẽ chia ra 8 vùng nhớ nhỏ, ví dụ 256bytes tương đương SIZE = 7 (7+1) thì mỗi subregion sẽ là 32Bytes. Giờ mình ghi bit 1 (nhớ là từng bit đó ví dụ bật bit 7 thì region 7) lên ở bit nào tương ứng với region đó bị disable (hay đồng nghĩa vùng nhớ đấy sẽ không thể xâm phạm ). Ví dụ 0x02 (0b0000 0010) sẽ disable Subregion 1 (32-63Bytes), trong khi đó các sub khác vẫn hoạt động bình thường.
