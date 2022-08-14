Wireless Weather Station Project
By: Alex Pak and Shrey Varma

This was made as our final project for the COSMOS summer program. We currently provided the code in this repository
used to program the microcontroller and sensors we utilized in this build. Our wireless weather station is self-sustaining
where it uses a 5V solar panel to power an MSP432 TI microcontroller as well as a Lithium Polymer battery for use in the dark. 
The solar panel is also encased in a 3D printed stand which is connected to a rotating wheel that is spun using a servo motor.
This was done so that the solar panel is able to automatically rotate to face the brightest light source where we utilized photoresistors for this.
The original build was going to use a BMP280 pressure/temp sensor; however, we decided to just use a temperature sensor instead.
Other sensors can be utilized to enhance the station such as a humidity sensor or an AQ sensor, the possibilities are endless.
Data from the sensors were outputted to the microcontroller which was uploaded to a local server that utilized graphs to display it. 

Link to Slides(More Detailed Info): https://docs.google.com/presentation/d/1PSUSFK6MqLPTG2-99QEV4jIOzdOmJadtggiggVlIRdQ/edit?usp=sharing
