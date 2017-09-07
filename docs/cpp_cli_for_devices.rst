===============================================================================
C++ Client Library - Devices
===============================================================================

Introduction
-------------------------------------------------------------------------------

This Devices Section describes how to use Device Client with the C++ ibmiotf client library. For help with getting started with this module, see `C++ Client Library - Introduction <https://github.com/ibm-watson-iot/iot-cpp/blob/master/README.md>`__. 

Using Constructor to create DeviceClient instance with Properties Object
-------------------------------------------------------------------------------

The constructor builds the device client instance, and accepts a Properties object containing the following definitions:

* org - Your organization ID. (This is a required field. In case of quickstart flow, provide org as quickstart.)
* domain - (Optional) The messaging endpoint URL. By default the value is "internetofthings.ibmcloud.com"(Watson IoT Production server)
* type - The type of your device. (This is a required field.)
* id - The ID of your device. (This is a required field.
* auth-method - Method of authentication (This is an optional field, needed only for registered flow and the only value currently supported is "token"). 
* auth-token - API key token (This is an optional field, needed only for registered flow).
* clientTrustStorePath - Path to Watson IoT Server Certificate.
* port - Port Number to use for connection. Two supported secure ports are 8883 and 443.


The Properties class has setter/getter methods to initialize the values which are used to interact with the Watson IoT Platform module. 

The following code shows a device client instantiation using properties object in a Quickstart mode.


.. code:: C++

	//Qick start mode
	Properties prop;
	prop.setorgId("quickstart");
	prop.setdeviceType("devicetest");
	prop.setdeviceId("haritestdevice");

	//Create DeviceClient Instance
	IOTP_DeviceClient qsClient(prop);



The following program shows a device client instantiation using properties object in a registered flow

.. code:: C++

	//Initialize properties instance
	Properties p;
	p.setorgId("Org-ID");
        p.setdomain("internetofthings.ibmcloud.com");
        p.setdeviceType("type");
        p.setdeviceId("id");
        p.setauthMethod("token");
        p.setauthToken("password");
        p.setPort("8883");
        p.settrustStore("iot-cpp-home/IoTFoudnation.pem")

	//Create DeviceClient Instance
	IOTP_DeviceClient client(p);



Using Constructor to create DeviceClient instance with Configuration File
--------------------------------------------------------------------------

Instead of including a Properties object directly, you can use a configuration file containing the name-value pairs for Properties. If you are using a configuration file containing a Properties object, use the following code format.

.. code:: C++

	//Instantiate DeviceClient using configuration file
	IOTP_DeviceClient client("../samples/device.cfg");
	

The content of the configuration file must be in the following format:

::

  {
	"Organization-ID": $orgId,
	"Domain": $domain,
	"Device-Type": $deviceType,
	"Device-ID": $deviceId,
	"Authentication-Method": $authMethod,
	"Authentication-Token": $authToken,
	"Port" : 8883 or 443,
	"clientTrustStorePath" : "iot-cpp-home/IoTFoundation.pem"
   }



Connecting to the Watson IoT Platform
----------------------------------------------------

Connect to the Watson IoT Platform by calling the *connect* function. 

Also, one can use the setKeepAliveInterval(int) method before calling connect() to set the MQTT "keep alive" interval. This value, measured in seconds, defines the maximum time interval between messages sent or received. It enables the client to detect if the server is no longer available, without having to wait for the TCP/IP timeout. The client will ensure that at least one message travels across the network within each keep alive period. In the absence of a data-related message during the time period, the client sends a very small "ping" message, which the server will acknowledge. A value of 0 disables keepalive processing in the client. The default value is 60 seconds.

.. code:: C++

	IOTP_DeviceClient client(prop);
	client.setKeepAliveInterval(90);
	std::cout<<"Connecting client to Watson IoT platform"<<std::endl;
	success = client.connect();
    
After the successful connection to the IoTF service, the device client can perform the following operations, like publishing events and subscribe to device commands from application.


Publishing events
-------------------------------------------------------------------------------
Events are the mechanism by which devices publish data to the Watson IoT Platform. The device controls the content of the event and assigns a name for each event it sends.

When an event is received by the IBM IoT Foundation the credentials of the connection on which the event was received are used to determine from which device the event was sent. With this architecture it is impossible for a device to impersonate another device.

Events can be published at any of the three `quality of service levels <https://docs.internetofthings.ibmcloud.com/messaging/mqtt.html#/>`_ defined by the MQTT protocol.  By default events will be published as qos level 0.

Publish event using user-defined quality of service
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Events can be published at higher MQTT quality of servive levels, but these events may take slower than QoS level 0, because of the extra confirmation of receipt. Also Quickstart flow allows only Qos of 0

.. code:: C++

	std::string jsonMessage;
	jsonMessage = "{\"Data\": {\"Temp\": \"34\" } }"; 
	client.publishEvent("status", "json", jsonMessage.c_str(), 1);


Handling commands
-------------------------------------------------------------------------------
When the device client connects it automatically subscribes to any command for this device. To process specific commands you need to register a command callback method. 
The messages are returned as an instance of the Command class which has the following properties:

* std::string deviceType;
* std::string deviceId;
* std::string commandName;
* std::string format;
* std::string payload;


.. code:: C++


	//Implement the CommandCallback class to provide the way in which you want the command to be handled
	class MyCommandCallback: public CommandCallback{
		/**
	 	* This method is invoked by the library whenever there is command matching the subscription criteria
	 	*/
		void processCommand(Command& cmd){
			std::cout<<"Received Command \n"
			<<"Command Name:"<<cmd.getCommandName()<<"\t format:"<<cmd.getFormat()<<" \t payload:"<<cmd.getPayload()<<"\n";
		}
	};

	//Registered device flow properties reading from configuration file in json format
	std::cout<<"Creating IoTP Client with properties"<<std::endl;
	IOTP_DeviceClient client(prop);
	client.setKeepAliveInterval(90);
	std::cout<<"Connecting client to Watson IoT platform"<<std::endl;
	success = client.connect();
	std::cout<<"Connected client to Watson IoT platform"<<std::endl;
	std::flush(std::cout);
	if(!success)
		return 1;

	MyCommandCallback myCallback;
	client.setCommandHandler(&myCallback);
	

For complete code sample, refer to our `DeviceSample <https://github.com/ibm-watson-iot/iot-cpp/blob/master/samples/sampleDevice.cpp>`_ Program.

----
