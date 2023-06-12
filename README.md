# picoConsole
A single-board "game console" with DVI output powered by RP2040.

当前硬件设计版本为`v0.5`。


## 硬件设计
原理图请参考[此处](./hardware/v0.5/Schematic.pdf)。
* 基于`RP2040`单片机，超频至252MHz
    * 一颗`W25QxxJVSIQ`系列SPI FLASH用于存储固件，运行于QPI模式下
    * 一个Micro SD卡槽用于外部存储，运行于SPI单线模式，最高运行频率50MHz
    * 一颗`LY68L6400SLIT` SPI SRAM用于缓存数据，能够以高于SD卡的速率和稳定性进行读写，运行于SPI单线模式，最高运行频率>100MHz（如果在未来的实际开发中认为SD卡读写速度已经满足需求时，可能会摒弃该设计）
    * 四颗LED灯用于Debug指示，其中LED_0～LED_2为绿色，LED_RED为红色
* 使用USB TYPE-C接口进行数据传输和供电，包含正确的CC电阻设计
* 使用HDMI接口用于DVI信号输出
    * 一颗`EM6K34T2CR`用于完成5V和3.3V电平的双向转换，使得RP2040能够读取HDMI EDID信息（未验证）
* 引出未占用的IO用于后期开发

## 相关依赖
* 开发环境为Arduino IDE
    * 使用[Arduino Pico开发板管理器](https://github.com/earlephilhower/arduino-pico)
    * [PicoDVI-Adafruit Fork](https://github.com/adafruit/PicoDVI)
        * Adafruit GFX Library
        * Adafruit BusIO

## TODO
- [ ] 根据[pull#45](https://github.com/Wren6991/PicoDVI/pull/45)对Adafruit的Fork进行魔改，从而支持HDMI音频输出
- [ ] 由`dvi_test`创建`Pixel-Style City`的Demo，创建完整的图层和动画逻辑
- [ ] 验证`EM6K34T2CR对I2C`电平转换的设计是否正确
- [ ] 加入输入设备的支持，考虑更加优雅的输入设备接口（初步考虑使用自定义针脚定义的TYPE-C接口）

## 针脚定义
<table>
 <tr align="center">
  <td>外围设备（硬件控制器）</td>
  <td>GPIO</td>
  <td>功能</td>
 </tr>
 
 <tr align="center">
  <td rowspan="5">SD卡（SPI0）</td>
  <td>GPIO0</td>
  <td>Card Detect</td>
 </tr>
 <tr align="center">
  <td>GPIO1</td>
  <td>Chip Select/CSn/CE#</td>
 </tr>
 <tr align="center">
  <td>GPIO2</td>
  <td>CLK/SCLK</td>
 </tr>
 <tr align="center">
  <td>GPIO3</td>
  <td>DI/MOSI/TX</td>
 </tr>
 <tr align="center">
  <td>GPIO4</td>
  <td>DO/MISO/RX</td>
 </tr>
 
 <tr align="center">
  <td>预留</td>
  <td>GPIO5-10</td>
  <td>由2.54mm排针引出</td>
 </tr>
 
 <tr align="center">
  <td rowspan="4">SRAM（SPI1）</td>
  <td>GPIO12</td>
  <td>SO/MISO/RX</td>
 </tr>
 <tr align="center">
  <td>GPIO13</td>
  <td>SI/MOSI/TX</td>
 </tr>
 <tr align="center">
  <td>GPIO14</td>
  <td>SCLK</td>
 </tr>
 <tr align="center">
  <td>GPIO15</td>
  <td>Chip Select/CSn</td>
 </tr>
 
 <tr align="center">
  <td rowspan="4">Debug灯</td>
  <td>GPIO16</td>
  <td>LED_2</td>
 </tr>
 <tr align="center">
  <td>GPIO17</td>
  <td>LED_1</td>
 </tr>
 <tr align="center">
  <td>GPIO18</td>
  <td>LED_0</td>
 </tr>
 <tr align="center">
  <td>GPIO19</td>
  <td>LED_RED</td>
 </tr>
 
 <tr align="center">
  <td rowspan="2">HDMI EDID（I2C0）</td>
  <td>GPIO20</td>
  <td>SDA</td>
 </tr>
 <tr align="center">
  <td>GPIO21</td>
  <td>SCL</td>
 </tr>
 
 <tr align="center">
  <td rowspan="8">DVI输出</td>
  <td>GPIO22</td>
  <td>TMDS CLK -</td>
 </tr>
 <tr align="center">
  <td>GPIO23</td>
  <td>TMDS CLK +</td>
 </tr>
 <tr align="center">
  <td>GPIO24</td>
  <td>TMDS D0 -</td>
 </tr>
 <tr align="center">
  <td>GPIO25</td>
  <td>TMDS D0 +</td>
 </tr>
 <tr align="center">
  <td>GPIO26</td>
  <td>TMDS D1 -</td>
 </tr>
 <tr align="center">
  <td>GPIO27</td>
  <td>TMDS D1 +</td>
 </tr>
 <tr align="center">
  <td>GPIO28</td>
  <td>TMDS D2 -</td>
 </tr>
 <tr align="center">
  <td>GPIO29</td>
  <td>TMDS D2 +</td>
 </tr>
</table>
