<!-- Official repository: https://github.com/Danmuse/InductometerTRD_FRBA -->
<!--
<details>
  <summary><strong><span>&#160;</span>References</strong></summary>
  <ul>
    <span>&#10038;</span> [STM32F103C8Tx interesting projects](https://github.com/CaliBeta).<br>
    <span>&#10038;</span> [Generate sinusiodal signal with Pulse-Width Modulation](https://www.youtube.com/watch?v=9wfItbD-dHI).<br>
    <span>&#10038;</span> [STM32 Blue-pill VS Arduino NANO](https://www.youtube.com/watch?v=mdxmfjVhEus).<br>
  </ul>
</details>
-->

## 📕️ __Overview__

The **Inductometer** project with autoranging aims to create a device to measure inductance and calculate the quality factor of any inductor from the resonance frequency of the circuit. This meter is ideal for educational, research and electronic design applications.

### 🔌️ __Hardware & Setup__

- [ ] [Identification of the sensors, modules and controllers used.](https://github.com/Danmuse/InductometerTRD_FRBA/issues/1) `DDM ~ 3`
<details>
  <summary><strong><span>&#160;</span>Electronic components</strong></summary>
  <ul>
    <span>&#10038;</span> <b>STM32F103C8T6 Blue-pill board</b>.<br>
    <span>&#10038;</span> Rechargeable polymer lithium battery.<br>
    <span>&#10038;</span> <b>TP4056</b> Charging module for 3.7V/1A Li-ion battery.<br>
    <span>&#10038;</span> <b>AD9833</b> Sinusoidal/triangular/square signal generator (0.1Hz ~ 1.25MHz).<br>
    <span>&#10038;</span> <b>TLV3501AID</b> High speed comparator with shutdown.<br>
    <span>&#10038;</span> <b>MT3608</b> Step up (2V ~ 28V/2A).<br>
    <span>&#10038;</span> <b>LM393</b> Operational amplifier.<br>
    <span>&#10038;</span> <b>X9C103S</b> 10KΩ digital potentiometer<br>.
    <span>&#10038;</span> <b>INA219</b> Current/voltage sensor (500mV ~ 26V/3.2A).<br>
    <!-- <span>&#10038;</span> <b>100Ω</b> 1/4W and <b>220Ω</b> 1/4W Resistors.<br>
    <span>&#10038;</span> Mx471 Current/Voltage sensor ~ Maximum 3A/3-36V. (Q-meter)<br>
    <span>&#10038;</span> ACS712T-5A Current sensor ~ Maximum 5A. (Q-meter)<br>
    <span>&#10038;</span> FZ0430 Voltage sensor ~ Maximum 25V. (Q-meter)<br> -->
    <span>&#10038;</span> Capacitor bank with different types.<br>
    <span>&#10038;</span> <b>KY-040</b> Rotary encoder.<br>
    <span>&#10038;</span> IPS HD TFT <b>ST7789</b>.<br>
    <span>&#10038;</span> Slide power switch.<br>
    <!-- <span>&#10038;</span> EEPROM Memory FM24C16U 16KB. (Optional)<br> -->
  </ul>
</details>

- [ ] [Hardware installations and cabinet design.](https://github.com/Danmuse/InductometerTRD_FRBA/issues/2) `DDM ~ 0`
- [ ] [Development board (Schematic and PCB).](https://github.com/Danmuse/InductometerTRD_FRBA/issues/3) `DDM ~ 0`

### 👨‍💻️ __Firmware & Software__

- [ ] [Achieving connectivity between sensors and modules with the microcontroller.](https://github.com/Danmuse/InductometerTRD_FRBA/issues/4) `DDM ~ 14` `MSS ~ 0`

### 📚️ __Documentation & Doxygen__

- [ ] [Doxygen v1.9.6 implementation for code documentation.](https://github.com/Danmuse/InductometerTRD_FRBA/issues/5) `DDM ~ 0`
- [x] [Presentation of the project idea.](https://github.com/Danmuse/InductometerTRD_FRBA/issues/6) `DDM ~ 4.5` `MSS ~ 1`
- [ ] [Comprehensive and extensive project report.](https://github.com/Danmuse/InductometerTRD_FRBA/issues/7) `DDM ~ 0.5` `MSS ~ 0`
- [ ] [Illustrations of the experience and trials.](https://github.com/Danmuse/InductometerTRD_FRBA/issues/8) `DDM ~ 0.5`
- [ ] [Load component datasheet.](https://github.com/Danmuse/InductometerTRD_FRBA/issues/9) `DDM ~ 2`

---

## 🌟️ __Contributing__

General Firmware Developers: __@Danmuse__  
General Hardware Developers: __@Danmuse__ ~ __@msanchezsosa__  
Documentation Managers: __@Danmuse__ ~ __@msanchezsosa__   
Doxygen Editor: __@Danmuse__  

<br>

<a href="https://github.com/Danmuse/InductometerTRD_FRBA/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=Danmuse/InductometerTRD_FRBA&max=2" />
</a>
