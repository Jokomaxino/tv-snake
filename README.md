<div id="top"></div>


<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/jokomaxino/tv-snake">
    <img src="images/SnakeLogo.png" alt="Logo" height='64' width='192'>
  </a>

<h3 align="center">tv-snake</h3>

  <p align="center">
    Play snake on an analog TV with Arduino
    <br />
    <br />
    <a href="https://github.com/jokomaxino/tv-snake/issues">Report Bug</a>
    ·
    <a href="https://github.com/jokomaxino/tv-snake/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a> </li>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
 Play snake on an analog TV with Arduino.  
 
 **Demo video**  
[![demo video](https://img.youtube.com/vi/z0wNSLzvSKA/0.jpg)](https://www.youtube.com/watch?v=z0wNSLzvSKA)  

**Screnshots**  
![intro](images/screenshot1.jpg)  
![gameplay](images/screenshot2.jpg)  
![game over](images/screenshot3.jpg)  

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

**Materials** 
 - Arduino Uno
 - potentiometer
 - wires
 - breadboard
 - joystick
 - analog TV (NTSC or PAL)
 - RCA cable
 

**Procedure**  
Install the Arduino IDE and copy the contents of libraries folder to your libraries. Wire an Arduino Uno this way:   
![](images/schematic.png)   

The default code works for an NTSC TV. If your have a PAL TV, change the line in [tv-snake.ino](snake/tv-snake.ino) that says   
        `TV.begin(NTSC, 120, 96);`  
to   
        `TV.begin(PAL, 120, 96);`  

NTSC or PAL depends on what country you are in.  
![NTSC or PAL countries](https://upload.wikimedia.org/wikipedia/commons/thumb/0/0d/PAL-NTSC-SECAM.svg/435px-PAL-NTSC-SECAM.svg.png)

Enjoy!

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- CONTRIBUTING -->
## Contributing

This isn't perfect, but it looks good. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Jose Enrico Maxino - jokomaxino@gmail.com

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* Code for the game was based on [snake for 8x8 LED matrix](https://create.arduino.cc/projecthub/rishab8551/arduino-snake-game-using-arduino-and-martix-6c230c)
* [TVout Library](https://www.arduino.cc/reference/en/libraries/tvout/) used for drawing to the screen.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/jokomaxino/tv-snake.svg?style=for-the-badge
[contributors-url]: https://github.com/jokomaxino/tv-snake/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/jokomaxino/tv-snake.svg?style=for-the-badge
[forks-url]: https://github.com/jokomaxino/tv-snake/network/members
[stars-shield]: https://img.shields.io/github/stars/jokomaxino/tv-snake.svg?style=for-the-badge
[stars-url]: https://github.com/jokomaxino/tv-snake/stargazers
[issues-shield]: https://img.shields.io/github/issues/jokomaxino/tv-snake.svg?style=for-the-badge
[issues-url]: https://github.com/jokomaxino/tv-snake/issues
[license-shield]: https://img.shields.io/github/license/jokomaxino/tv-snake.svg?style=for-the-badge
[license-url]: https://github.com/jokomaxino/tv-snake/blob/master/LICENSE.txt
