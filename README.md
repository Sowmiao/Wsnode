# Wsnode

/** Introduction**/

1)Designed and developed software in C and C++ to configure Arduino Uno and Arduino mega Shield as a wireless Sensor node


2)As well, implemented a design to parse data and send data through Arduino Uno and Arduino mega Shield (which was configured as a wireless sensor node) onto the gateway/Co-ordinator (From where the data was sent to the particle.io cloud or any other client cloud). 


3)Achieved implementing this design at a very low power when compared to conventional systems which are into the same domain. 


4)This was possible by exploiting the sleep-wake cycle[will not be including a code for this due to certain constarints within the organization, but will give a brief over how it was done soon] of the microprocessor(MSP-EXP430) and also due to the use of Radio network(CC1120) which could be used as a Sub-1Ghz wireless mesh Node, when implemented together.

These attributes how they were done on Linux , will update soon.

•Analysed the data that was sent to the cloud on a real time basis and performed required 
troubleshooting operations to manage the cloud.(Linux Based environment troubleshooting
(Cygwin Terminal))


Also would update on how the hardware testing was done.

•Tested the configurations and the activities of the configured node using 
MSP-EXP430F5529LP(Hardware testing)


5)Finally, implemented test cases to check if the data sent is error free by using the concept of cyclic redundancy check. Also checked if the gateway receive's the exact same data which was sent without any loss.




