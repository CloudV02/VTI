

Khóa học này mình sẽ kết hợp với khóa học trước để hiểu sâu các vấn đề, và thường sẽ là phân tích lời giảng của người ta.

<h1><summary>3. AUTOSAR Basics<summary></h1>
<details>
<h2><summary>3.1.AUTOSAR Introduction<summary></h2>
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

<h2><summary>3.2.AUTOSAR Layered Architecture<summary></h2>
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

</details>
</details>