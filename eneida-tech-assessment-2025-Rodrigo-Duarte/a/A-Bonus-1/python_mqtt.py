import paho.mqtt.client as mqtt
import json
import time
import random
import math
from datetime import datetime

class MQTTSimulator:
    def __init__(self, broker_host="localhost", broker_port=1883, topic="sensors/rms"):
        self.broker_host = broker_host
        self.broker_port = broker_port
        self.topic = topic
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        self.client.on_publish = self.on_publish
        
    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print(f"Connected to MQTT broker at {self.broker_host}:{self.broker_port}")
        else:
            print(f"Failed to connect to MQTT broker. Return code: {rc}")
    
    def on_disconnect(self, client, userdata, rc):
        print("Disconnected from MQTT broker")
    
    def on_publish(self, client, userdata, mid):
        print(f"Message published with ID: {mid}")
    
    
    def create_message(self):
        timestamp = datetime.now().isoformat()
        rms_value = round(random.uniform(220.0, 240.0), 2) 
        
        message = {
            "timestamp": timestamp,
            "sensor_id": "RMS_001",
            "measurement_type": "RMS_voltage",
            "value": rms_value,
            "unit": "V",
        }
        
        return json.dumps(message, indent=2)
    
    def connect(self):
        """Connect to MQTT broker"""
        try:
            self.client.connect(self.broker_host, self.broker_port, 60)
            self.client.loop_start()
            return True
        except Exception as e:
            print(f"Error connecting to broker: {e}")
            return False
    
    def disconnect(self):
        self.client.loop_stop()
        self.client.disconnect()
    
    def publish_rms_data(self):
        message = self.create_message()
        result = self.client.publish(self.topic, message, qos=1)
        
        if result.rc == mqtt.MQTT_ERR_SUCCESS:
            print(f"Published to {self.topic}:")
            print(message)
            print("-" * 50)
        else:
            print(f"Failed to publish message. Error code: {result.rc}")
    
    def run_simulation(self, duration_seconds=None):
        if not self.connect():
            return
        
        print(f"Starting MQTT RMS simulation...")
        
        start_time = time.time()
        
        try:
            while True:
                self.publish_rms_data()
                

                if duration_seconds and (time.time() - start_time) >= duration_seconds:
                    print(f"Simulation completed after {duration_seconds} seconds")
                    break
                
                time.sleep(5)
                
        except KeyboardInterrupt:
            print("\nSimulation stopped by user")
        except Exception as e:
            print(f"Error during simulation: {e}")
        finally:
            self.disconnect()

def main():

    BROKER_HOST = "localhost"  
    BROKER_PORT = 1883
    TOPIC = "sensors/rms"
    
    simulator = MQTTSimulator(BROKER_HOST, BROKER_PORT, TOPIC)
    
    simulator.run_simulation()
    


if __name__ == "__main__":
    main()