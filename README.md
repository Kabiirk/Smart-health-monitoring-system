# **Smart-health-monitoring-system**

### **PROBLEM STATEMENT**
As older adults become more physically frail, they’re less able to take care of themselves. Mental or physical ailments can make them less Susceptible to changes in their body conditions. And they may not see or hear as well or think as clearly as they used to, leaving openings for various ailments etc. To aid the elderly in their health-keeping and allow them to take care of themselves, a monitoring system is required that allows a third (Or even the elderly themselves) to quantitatively monitor their health and other conditions.So that when the time comes, appropriate action can be taken for their well-being

### **ABSTRACT**
The advancement of the Internet of Things technology is playing a key role in developing the health sector by making it much more accessible and affordable through easy to use applications for virtual and distant interactions with patients. Taking the capability of IoT technology into account, it is possible to overcome the difficulties faced by physically unstable or old patients in consulting a doctor physically on a regular basis. This work has led to a prototype of IoT Based Remote Health Monitoring System for Patients This also reduces the medical expenses for patients significantly. In addition, the doctors can prescribe necessary medications by observing the patient's health stats over time through an application. Detailed analysis of the signals was obtained with respect to variations in physical and environmental activities to understand the functioning of the sensors used.

### **WHY ARE THE ELDERLY MORE SUSCETIBLE TO ILLNESSES?**
Older patients are more susceptible to infections because their immune systems are weaker. They also may have additional conditions that open the door for infection. Chronic age-related diseases can contribute to weakness as well. Lastly, the elderly patient is likely to be on more than one medication, which can affect how the body protects itself against infections
And Some major diseases experienced by them are:

#### **Heat Strokes**
* When we age, our bodies become less efficient at regulating temperature for a couple of reasons. The Elderly don’t sweat as much as younger adults, which is one of the body’s most important heat-regulation mechanisms.
* Well, as the temperature rises, so too does your internal body temperature, especially when you’re exposed directly to the sun or extremely hot environments. Which is why seniors suffer from heat stroke more often than younger people throughout the summer.
* Early warning signs of heat exhaustion, which may precede the more serious heat stroke, include excessive sweating, tiredness, weakness, dizziness, headache and muscle cramps.

#### **Heart problems**
* Aging can cause changes in the heart and blood vessels. For example, as you get older, your heart can't beat as fast during physical activity or times of stress as it did when you were younger. However, the number of heartbeats per minute (heart rate) at rest does not change significantly with normal aging.
* The most common aging change is increased stiffness of the large arteries, called arteriosclerosis or hardening of the arteries. This causes high blood pressure, or hypertension, which becomes more common as we age.
* Another major symptom is arrythmia i.e. irregular heartbeat which can be measured by a pulse sensor.

#### **Orientational problems**
* Perceptual system becomes less efficient with age.
* Visual perception relies on low-level encoding of local orientation. Recent studies show an age-dependent impairment in orientation discrimination of stimuli embedded in external noise, suggesting that encoding of orientation is inefficient in older adults. 
* This means that as you get older, your ‘sense of’ balance becomes weaker too.
* This problem is also compounded by other ailments like Alzheimer’s which affects your motor-muscular systems the most. 

### **PROPOSED SOLUTION**
An IoT based device which is an SoC capable of connectivity as well. The main purpose of that Soc would be to collect data from multiple sensors and send that data to a cloud storage, from where, a dedicated app draws the data and can help visualize it and out it in user-perspective. This App can be anywhere in the world, the only requirement for it is that the device the app is on should be connected to the net via data or by wifi.
The SoC needs to be connected to the net as well for the purpose of transmitting data, but it need not be connected to the same network the SoC is connected to. This decoupling occurs since data isn’t send directly to the app, rather to a server where the app draws the data from. This decoupling also adds to the potability of the solution.

### COMPONENTS
- **HARDWARE**
	- ESP8266
	- DHT11
	- Pulse Sensor, MPU6050
	- Breadboard/ Development Boards
	
- **SOFTWARE**
	- Blynk Server
	- Push Notification Service
	- Arduino IDE (For Development and coding)
