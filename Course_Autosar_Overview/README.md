

Khóa học này mình sẽ kết hợp với khóa học trước để hiểu sâu các vấn đề, và thường sẽ là phân tích lời giảng của người ta.

<h1><summary>3. AUTOSAR Basics</summary></h1>
<details>
<h2><summary>3.1.AUTOSAR Introduction</summary></h2>
<details>
- Nói chung là phần này giới thiệu vớ vẩn thoi, đọc bài của anh Nghĩa là đủ

The classic Autosar platform runs on a microcontroler and is divided into 3 main layers:
- Basuc Software (BSW)
- AUTOSAR Runtime Evironment(RTE)
- Appication Layer

Need for Autosar?
Bởi vì là nếu như ngày trước mà không có Autosar thì phần cứng và phần mềm nó sẽ liên kết chặt chẽ với nhau, thay phần cứng cái thì phần mềm cũm sẽ phải viết lại từ đầu. Nên vì thế người ta cần Autosar để độc lập giữa các lớp Hardware và Software.

So the main motto of Autosar is to cooperate on standards and compete on implementation(cạnh tranh về việc thực hiện). Nói vậy nghĩa là sao? Cooperate on standards tức là các công ty OEM cùng nhau ngồi lại để tạo ra chuẩn Autosar để các nhà cung cấp có thể dễ dàng hợp tác với nhiều công ty chẳng hạn. Còn compete tức là cạnh tranh để thực hiện nó ra sao, tức là 1 số công ty có những con chip mạnh hơn để xử lý cảm biến hoặc 1 số cty ô tô sẽ nhiều tính năng tiện ích như cửa sổ trời các thứ, thì đó chính là do các hãng người ta phát triển.

For the benefit for each department:

- OEM: Same software can be reused for different variants of cars (có thể tái sử dụng cùng 1 phần mềm cho nhiều loại xe khác nhau). And they main ability to compete on innovative functions and the flexibility is also increased (và khả năng cạnh tranh chính của họ về các chức năng sáng tạo và tính linh hoạt cũm được tăng lên). And also the software developemnt cost is also reduced
- Supplier: The efficiency would be improved. New business models are possible. Development partitioning among suppiers.
Nguyễn Hữu Nghĩa say:
Thì các Layer trên cũm được phân hóa về mặt phần mềm để cho các công ty / nhà sản xuất khác nhau có thể dễ dàng tham gia vào quá trình phát triển phần mềm xe. Nên nó sẽ tạo ra 1 số các thuật ngữ cơ bản về các nhà cung cấp phần cứng/phần mềm:
- OEM - Original Eqipment Manufacturer
- Tier 1:
- Tier 2:
Đọc kĩ hơn ở link: https://www.laptrinhdientu.com/2023/01/Autosar01.html
</details>

<h2><summary>3.2.AUTOSAR Layered Architecture</summary></h2>
<details>
- Application layer: So your application code, it sits in this application layer.
- Runtime evirionment is like a virtual function bus, which makes your application independent of the below layer this part is called BSW that basic software.
- BSW: consists of service layer,ECU absraction layers and lowest is your microcontroller and above microcontroller we have MCAL. And this is BSW complete.
    - The MCAL is the lowest software layer of the Basic Software. It contains internal drivers, which are software modules with direct access to microcontroller and internal peripherals.
    - The ECU Abstraction Layer interfaces(tương tác) the drivers of the Microcontroller Abstraction Layer (tức là nó sẽ lấy cái trừu tượng của lớp MCAL). It also contains drivers for external devices. It offers an API for access to periperal and devices regardless of (bất kể) their location (microcontrol internal or external) and their connection to the microcontroller (port pins, type of interface) (có nghĩa là nó không phụ thuộc vào vị trí của internal or external peripheral or connection to microcontroller, kiểu mình không cần quan tâm tới cái địa chỉ mà mình trỏ tới mà chỉ cần thông qua API như DIO_Write chẳng hạn, í nó là vậy).
    - CDD The Complex Drivers, CDD Layers spans(trải dài) from the hardware to RTE(tức là nó sẽ không liên quan gì đến các lớp trong BSW và nó chạy thẳng đến RTE luôn). They provide the possibility to integrate specical purpose functionality, eg. drivers for devices:
        - which are not specified within AUTOSAR.
        - with very high timing constrains
        - for migration purposes etc 
        - might be application, microcontroller, and ECU hardware dependent
    - The Service Layer is the highest layer og the Basic Software which aloso applies for its relevance for the application software: while access I/O signals is covered by ECU Abstraction Layer, the Service Layer offers:
        - Operating system functionality
        - Vehicle network comunication and management services. (Com module)
        - Memory services (NVRAM management)
        - Diagnostic Services (include UDS Comumunication, error memory and fault treatment)
        - ECU state management, mode management.
        - Logical and temporal program flow monitoring(watchdog manager) (Giám sát luồng chương trình logic và thời gian)
    -> So this is like the brains because your OS is there and your complete management of your network communication, all those is this layer. So basically it provides basic services for the appilcation, RTE and basic software modules.

- RTE is a layer providing communication services to application software. It is an interface between your application and your BSW, and it make your application independent with BSW, or you can sau underlying layers. It acts as a virtual function bus. So above the software of your application, it is component. So we the term software components. So each application is written in components called as software components and the Autosar software component can communicate with other components. So within our ECU it can communicate or within our different ECUs also then con communicate and makes the software component independent from the mapping to a specific ECU. (Í muốn nói là SWC sẽ được thiết kế sao cho không phụ thuộc vào việc nó mapping hay chạy trên bất cứ ECU nào là nhờ cái RTE đóa).

- The Basic Software can be subdivided into the following types of services:
    - Input/Output (I/O): Standardized access to sensors , actuators and ECU board peripherals.
    - Memory: Standardized access to interal/external memory (non volatile memory - NVM là bộ nhớ không mất dữ liệu khi tắt nguồn). 
    - Crypto: Standardized(chuẩn hóa) access to cryptographic primitives(mã hóa) including internal/external hardware accelerators (này í muốn nói là ECU có chip hộ trợ mã hóa như phần cứng AES, Crypto module có thể dùng phần đó để chạy nhanh hơn). We need it for security purposes.
    - Communication: Standardized access to vehicle network systems, ECU onboard communication systems and ECU internal SW
    - Off-board Communication: Standardized access to: Vehicle-to-X communication, in vehicle wireless network systems, ECU off-board communication systems
    - System: Provision of standarddizeable (OS, timers, error memory) and ECU specific (ECU state management, watchdog manager) services and library functions 

</details>

<h2><summary>3.3. Configuration Classes and Interfaces in Autosar</summary></h2>
<details>

The first one is pre compile time and this we have proprocessor instructions. Code generation based on this preprocessor instructions is done.

**Pre compile time**

Uses cases:
- Enable/disabling optional functionality (tức là có là thế bật tắt các chức năng không bắt buộc): This allows to exclude(loại trừ) parts of the source code that node needed.
- Optimize of performance and code size.

Restrictions:
- Using #define results in most cases in more efficient code then access to constants or even access to constants via pointers. Generated code avoids code and runtime overhead.
- The module must be avaiable as source code(.c/.h).
- The configuration is static, to change the configuration, the module has to be recompiled (quan trọng nhất thì trong autosar nếu mà làm với pre compile time thì mỗi lần thay đổi cái gì lại phải chạy lại từ đầu nên từ đóa mới sinh ra post-build)

Configuration files (*_Cfg.h, *_Cfg.c)
- *_Cfg.h stores macros and #define
- *_Cfg.c stores e.g constants

-> Tất cả sẽ được cấu hình cố định trong file .h.c, không thay đổi được nếu không rebuild
Example:
```C
#define DIO_CHANNEL_LED     0x01
#define DIO_CHANNEL_BUTTON  0x02
```
-> Nếu muốn thay đổi BUTTON thành kênh khác, phải rebuild code.

**Link time**
- Constant data outside the module, the data can be configured after the module has been compiled.
So once compilation is done, we can change this data. So in previous one (pre compile time) to change anything in the code, we had to recompile everything. But in this case, only the outside data can be modified.
Use cases:
- Configuration of modules that are only avaiable as object code(.o)
- Creation of configuration after compilation but befor linking
- Selection of configuration set after compilation but before linking.

Example implementation: (Example)
- If we have one configuration set, then in runtime we will not have any selection -> this is configurarion set.
- The data relate to it or configuration data will be captured in external constant (.c file), and these external constants are located in a separate file and the module will have direct acces to these external.

-> tức là ở VD này ta sẽ có 2 file, 1 file chứa cái configuration và cái file này trong quá trình runtime nó sẽ không tìm được data, vì ta đã config data cho nó đâu. Và 1 file.c khác (hoặc có thể là nhiều file nhưng trong ví dụ này là 1), thì này là configuration data nó sẽ được nằm trong 1 cái file cụ thể và cái module hay cái configuration set sẽ trỏ tới cái file.c chứa configuration data để lấy dữ liệu để config. Và để 2 file.c liên kết với nhau thì cta phải build ra file.o và các config đã phải sẵn sàng hết trước khi link.
-> Nếu thay đổi config vẫn phải rebuild nhưng không thay đổi toàn bộ như pre compile time mà chỉ rebuild cái configuration data thoi.

**Post build time**

- Loadable constant data outside the module. It is very similar to the previous link time, but this data, it is located in a specific memory segment that allows reloading. Ex: Reflashing in ECU production line (nạp lại ECU trong dây chuyền sản xuất)

- So in this single or multiple configuration sets can be provided (tức là có nhiều configuration sets được chọn trong run-time thay vì chỉ có 1 configuration set như link-time và link-time sẽ dựa vào config sau quá trình build khi mà compile hết xong tạo ra file .o đó, chứ không phải quá trình run-time như post build )

Uses cases:

In the case of link-time, we have this constant data in a file, but here we have it in a specific memory. So even in the runtime, multiple configuration sets can be provided.

- Configuration of data where only the structure is defined but the contents not known during ECU-build-time (tức là nó có cái structure trong bên trong nhưng cái nội dung trong đó thif không xác định vì nó có thể thay đổi trong quá trình run-time)
- Configuration of data that is likely to change change or has to adapted after ECU-build time
- Reusability of ECUs across different car versions (same application, different configuration)
Example: ECU in a low-cost car version may transmit less signals on the bus then the same ECU in a luxury car version.

**What are AUTOSAR Libraries**

Libraries are a collection of functions for related purposes, so these libraries can be called by modules, including software components ..

Thực ra cũm dell hiểu lắm

**AUTOSAR Interface**

![System Diagram](./AUTOSAR_Interface.png)

- Thì AUTOSAR Interface sẽ là kiểu nói về giao tiếp giữa các SWCs hoặc SWC với BSW thông qua việc mình định nghĩa port. Và cái AUTOSAR Interface này chính là nơi mình định nghĩa các port đóa, còn các interface mà các port hay liên kết là khác nó nằm trong AR-PACKAGE (là do Autosar quản lý), không thuộc SWC nào cả.

- Standardized Interface: là 1 cái interface là được định nghĩa từ trước bởi tiêu chuẩn Autosar được coi như là API giữ các lớp BSW và sử dụng ngôn ngữ C. Tức là nó là mấy cái DIO_Write .. tiêu chuẩn đóa, thì mấy cái tiêu chuẩn đóa là do mình viết, nhưng đối với những người viết layer trên như SWC là nó được định nghĩa từ trước ròi. Đóa thì ngoài việc sử dụng giữa các SWC, nó còn sử dụng giữa RTE và OS hoặc RTE và BSW module.

- Standardized AUTOSAR Interface: Thì đây là 1 Interface đặc biệt của Autosar, nó sẽ có các hàm được định nghĩa bởi Autosar standard. Và nó được sử dụng cho việc các SWC truy cập vào các AUTOSAR service( nó nằm ở lớp BSW - Service Layer) như ECU State Manager hay Chuẩn đoán Diognostic Event Manager.

</details>

<h2><summary>3.4. Configuration Classes and Interfaces in Autosar</summary></h2>
<details>


</details>
</details>