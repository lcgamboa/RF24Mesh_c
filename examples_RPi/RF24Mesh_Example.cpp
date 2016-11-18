 
 
 /** RF24Mesh_Example.cpp by TMRh20
  * 
  * Note: This sketch only functions on -RPi-
  *
  * This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
  * master node.
  * In this sketch, the nodes will refresh their network address as soon as a single write fails. This allows the
  * nodes to change position in relation to each other and the master node.
  *
  */
  
#include "RF24Mesh/RF24Mesh.h"  
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>


RF24 radio;  
RF24Network network;
RF24Mesh mesh;

uint32_t displayTimer=0;

int main(int argc, char** argv) {
RF24_init2(&radio,RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);  
RF24N_init(&network,&radio);
RF24M_init(&mesh,&radio,&network);
  
  // Set the nodeID to 0 for the master node
  RF24M_setNodeID(&mesh,4);
  // Connect to the mesh
  printf("start nodeID %d\n",RF24M_getNodeID(&mesh));
  RF24M_begin(&mesh);
  RF24_printDetails(&radio);

while(1)
{
  
  // Call mesh.update to keep the network updated
  RF24M_update(&mesh);
    
  // Send the current millis() to the master node every second
  if(millis() - displayTimer >= 1000){
    displayTimer = millis();
    
    if(!RF24M_write(&mesh,&displayTimer,'M',sizeof(displayTimer))){
       
      // If a write fails, check connectivity to the mesh network
      if( ! RF24M_checkConnection(&mesh) ){
        // The address could be refreshed per a specified timeframe or only when sequential writes fail, etc.
		printf("Renewing Address\n");
        RF24M_renewAddress(&mesh); 
	  }else{
        printf("Send fail, Test OK\n"); 
      }
    }else{
	  printf("Send OK: %u\n",displayTimer);
	}
  }
  delay(1);
  }
return 0;
}

      
      
      
