/////////////////////////////////////////////////////////////////////////////
/// @file client.h
/// Declaration of MQTT client class
/// @date May 1, 2013
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2013-2016 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_client_h
#define __mqtt_client_h

#include "async_client.h"

#include <string>
#include <memory>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Lightweight client for talking to an MQTT server using methods that block
 * until an operation completes.
 */
class client
{
	/** The default quality of service */
	static const int DFLT_QOS;
	/** The actual client */
	async_client cli_;
	/**
	 * The longest amount of time to wait for an operation (in milliseconds)
	 */
	int timeout_;

	/** Non-copyable */
	client() =delete;
	client(const async_client&) =delete;
	client& operator=(const async_client&) =delete;

public:
	/** Smart pointer type for this object */
	using ptr_t = std::shared_ptr<client>;
	/** Type for a collection of filters */
	using topic_filter_collection = async_client::topic_filter_collection;
	/** Type for a collection of QOS values */
	using qos_collection = async_client::qos_collection;

	/**
	 * Create a client that can be used to communicate with an MQTT server.
	 * This uses file-based persistence in the current working directory.
	 * @param serverURI
	 * @param clientId
	 */
	client(const std::string& serverURI, const std::string& clientId);
	/**
	 * Create a client that can be used to communicate with an MQTT server.
	 * This uses file-based persistence in the specified directory.
	 * @param serverURI
	 * @param clientId
	 * @param persistDir
	 */
	client(const std::string& serverURI, const std::string& clientId,
		   const std::string& persistDir);
	/**
	 * Create a client that can be used to communicate with an MQTT server.
	 * This allows the caller to specify a user-defined persistence object,
	 * or use no persistence.
	 * @param serverURI
	 * @param clientId
	 * @param persistence The user persistence structure. If this is null,
	 *  				  then no persistence is used.
	 */
	client(const std::string& serverURI, const std::string& clientId,
		   iclient_persistence* persistence);

	/**
	 * Virtual destructor
	 */
	virtual ~client() {}
	/**
	 * Close the client and releases all resource associated with the
	 * client.
	 */
	virtual void close();
	/**
	 * Connects to an MQTT server using the default options.
	 */
	virtual void connect();
	/**
	 * Connects to an MQTT server using the specified options.
	 * @param options
	 */
	virtual void connect(connect_options options);
	/**
	 * Disconnects from the server.
	 */
	virtual void disconnect();
	/**
	 * Disconnects from the server.
	 */
	virtual void disconnect(long quiesceTimeout);
	/**
	 * Returns a randomly generated client identifier based on the current
	 * user's login name and the system time.
	 */
	//static std::string generate_client_id();
	/**
	 * Returns the client ID used by this client.
	 * @return std::string
	 */
	virtual std::string get_client_id() const;
	/**
	 * Returns the delivery tokens for any outstanding publish operations.
	 */
	virtual std::vector<idelivery_token_ptr> get_pending_delivery_tokens() const;
	/**
	 * Returns the address of the server used by this client, as a URI.
	 * @return std::string
	 */
	virtual std::string get_server_uri() const;
	/**
	 * Return the maximum time to wait for an action to complete.
	 * @return int
	 */
	virtual int get_time_to_wait() const;
	/**
	 * Get a topic object which can be used to publish messages.
	 * @param tpc
	 * @return topic
	 */
	virtual topic get_topic(const std::string& tpc);
	/**
	 * Determines if this client is currently connected to the server.
	 * @return bool
	 */
	virtual bool is_connected() const;
	/**
	 * Publishes a message to a topic on the server and return once it is
	 * delivered.
	 * @param top The topic to publish
	 * @param payload The data to publish
	 * @param n The size in bytes of the data
	 * @param qos
	 * @param retained
	 */
	virtual void publish(const std::string& top, const void* payload, size_t n,
						 int qos, bool retained);
	/**
	 * Publishes a message to a topic on the server.
	 * @param top The topic to publish on
	 * @param msg The message
	 */
	virtual void publish(const std::string& top, const_message_ptr msg);
	/**
	 * Publishes a message to a topic on the server.
	 * @param top The topic to publish on
	 * @param msg The message
	 */
	virtual void publish(const std::string& top, const message& msg);
	/**
	 * Sets the callback listener to use for events that happen
	 * asynchronously.
	 * @param cb The callback functions
	 */
	virtual void set_callback(callback& cb);
	/**
	 * Set the maximum time to wait for an action to complete
	 * @param timeToWaitInMillis
	 */
	virtual void set_time_to_wait(int timeToWaitInMillis);
	/**
	 * Subscribe to a topic, which may include wildcards using a QoS of 1.
	 * @param topicFilter
	 */
	virtual void subscribe(const std::string& topicFilter);
	/**
	 * Subscribes to a one or more topics, which may include wildcards using
	 * a QoS of 1.
	 * @param topicFilters A set of topics to subscribe
	 */
	virtual void subscribe(const topic_filter_collection& topicFilters);
	/**
	 * Subscribes to multiple topics, each of which may include wildcards.
	 * @param topicFilters A collection of topics to subscribe
	 * @param qos A collection of QoS for each topic
	 */
	virtual void subscribe(const topic_filter_collection& topicFilters,
						   const qos_collection& qos);
	/**
	 * Subscribe to a topic, which may include wildcards.
	 * @param topicFilter A single topic to subscribe
	 * @param qos The QoS of the subscription
	 */
	virtual void subscribe(const std::string& topicFilter, int qos);
	/**
	 * Requests the server unsubscribe the client from a topic.
	 * @param topicFilter A single topic to unsubscribe.
	 */
	virtual void unsubscribe(const std::string& topicFilter);
	/**
	 * Requests the server unsubscribe the client from one or more topics.
	 * @param topicFilters A collection of topics to unsubscribe.
	 */
	virtual void unsubscribe(const topic_filter_collection& topicFilters);
};

/** Smart/shared pointer to an MQTT synchronous client object */
using client_ptr = client::ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_client_h

