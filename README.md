<div align=center>
<img src="https://github.com/BestModules-Libraries/img/blob/main/BMH12M205_V1.0.png" width="580" height="240"> 
</div> 

BMH12M205 
===========================================================


BMH12M205 is the weighing sensor module of Best Modules, using UART communication method. This document provides an explanation of the BMH12M205 Arduino Lib function and how to install Arduino Lib. This example demonstrates the function of reading weight, etc.

This library can be installed via the Arduino Library manager. Search for **BMH12M205**. 

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager. 

Documentation 
-------------------

* **BMH12M205 Arduino Library Description]( https://www.bestmodulescorp.com/bmh12m205.html#tab-product2 )** - Arduino Library Description.

Version History  
-------------------

* **V1.0.1**  
&emsp;&emsp;- Initial public release.
* **V1.0.2**  
&emsp;&emsp;- Change the usage of the readWeight() function: The host sends CMD to obtain the weight value;  
&emsp;&emsp;- Modify the example readWeight:Turn off automatic output, and the host sends CMD to obtain the weight value;  
&emsp;&emsp;- Add example readWeight_AutoOutput:Turn on automatic output, host scans data packets, and outputs weight values.

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact technical support on our BEST MODULES CORP. Email:service@bestmodulescorp.com

Distributed as-is; no warranty is given.

BEST MODULES CORP.
