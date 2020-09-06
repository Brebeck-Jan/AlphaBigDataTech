<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html lang="en">
<head>
<title>Source code</title>
<link rel="stylesheet" type="text/css" href="../../../../../../stylesheet.css" title="Style">
</head>
<body>
<div class="sourceContainer">
<pre><span class="sourceLineNo">001</span>/**<a name="line.1"></a>
<span class="sourceLineNo">002</span> * Licensed to the Apache Software Foundation (ASF) under one<a name="line.2"></a>
<span class="sourceLineNo">003</span> * or more contributor license agreements.  See the NOTICE file<a name="line.3"></a>
<span class="sourceLineNo">004</span> * distributed with this work for additional information<a name="line.4"></a>
<span class="sourceLineNo">005</span> * regarding copyright ownership.  The ASF licenses this file<a name="line.5"></a>
<span class="sourceLineNo">006</span> * to you under the Apache License, Version 2.0 (the<a name="line.6"></a>
<span class="sourceLineNo">007</span> * "License"); you may not use this file except in compliance<a name="line.7"></a>
<span class="sourceLineNo">008</span> * with the License.  You may obtain a copy of the License at<a name="line.8"></a>
<span class="sourceLineNo">009</span> *<a name="line.9"></a>
<span class="sourceLineNo">010</span> *     http://www.apache.org/licenses/LICENSE-2.0<a name="line.10"></a>
<span class="sourceLineNo">011</span> *<a name="line.11"></a>
<span class="sourceLineNo">012</span> * Unless required by applicable law or agreed to in writing, software<a name="line.12"></a>
<span class="sourceLineNo">013</span> * distributed under the License is distributed on an "AS IS" BASIS,<a name="line.13"></a>
<span class="sourceLineNo">014</span> * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.<a name="line.14"></a>
<span class="sourceLineNo">015</span> * See the License for the specific language governing permissions and<a name="line.15"></a>
<span class="sourceLineNo">016</span> * limitations under the License.<a name="line.16"></a>
<span class="sourceLineNo">017</span> */<a name="line.17"></a>
<span class="sourceLineNo">018</span><a name="line.18"></a>
<span class="sourceLineNo">019</span>package org.apache.hadoop.hbase.replication;<a name="line.19"></a>
<span class="sourceLineNo">020</span><a name="line.20"></a>
<span class="sourceLineNo">021</span>import java.util.List;<a name="line.21"></a>
<span class="sourceLineNo">022</span>import java.util.Map;<a name="line.22"></a>
<span class="sourceLineNo">023</span>import java.util.Set;<a name="line.23"></a>
<span class="sourceLineNo">024</span><a name="line.24"></a>
<span class="sourceLineNo">025</span>import org.apache.hadoop.hbase.TableName;<a name="line.25"></a>
<span class="sourceLineNo">026</span>import org.apache.yetus.audience.InterfaceAudience;<a name="line.26"></a>
<span class="sourceLineNo">027</span><a name="line.27"></a>
<span class="sourceLineNo">028</span>/**<a name="line.28"></a>
<span class="sourceLineNo">029</span> * For creating {@link ReplicationPeerConfig}.<a name="line.29"></a>
<span class="sourceLineNo">030</span> */<a name="line.30"></a>
<span class="sourceLineNo">031</span>@InterfaceAudience.Public<a name="line.31"></a>
<span class="sourceLineNo">032</span>public interface ReplicationPeerConfigBuilder {<a name="line.32"></a>
<span class="sourceLineNo">033</span><a name="line.33"></a>
<span class="sourceLineNo">034</span>  /**<a name="line.34"></a>
<span class="sourceLineNo">035</span>   * Set the clusterKey which is the concatenation of the slave cluster's:<a name="line.35"></a>
<span class="sourceLineNo">036</span>   * hbase.zookeeper.quorum:hbase.zookeeper.property.clientPort:zookeeper.znode.parent<a name="line.36"></a>
<span class="sourceLineNo">037</span>   */<a name="line.37"></a>
<span class="sourceLineNo">038</span>  ReplicationPeerConfigBuilder setClusterKey(String clusterKey);<a name="line.38"></a>
<span class="sourceLineNo">039</span><a name="line.39"></a>
<span class="sourceLineNo">040</span>  /**<a name="line.40"></a>
<span class="sourceLineNo">041</span>   * Sets the ReplicationEndpoint plugin class for this peer.<a name="line.41"></a>
<span class="sourceLineNo">042</span>   * @param replicationEndpointImpl a class implementing ReplicationEndpoint<a name="line.42"></a>
<span class="sourceLineNo">043</span>   */<a name="line.43"></a>
<span class="sourceLineNo">044</span>  ReplicationPeerConfigBuilder setReplicationEndpointImpl(String replicationEndpointImpl);<a name="line.44"></a>
<span class="sourceLineNo">045</span><a name="line.45"></a>
<span class="sourceLineNo">046</span>  /**<a name="line.46"></a>
<span class="sourceLineNo">047</span>   * Sets a "raw" configuration property for this replication peer. For experts only.<a name="line.47"></a>
<span class="sourceLineNo">048</span>   * @param key Configuration property key<a name="line.48"></a>
<span class="sourceLineNo">049</span>   * @param value Configuration property value<a name="line.49"></a>
<span class="sourceLineNo">050</span>   * @return {@code this}<a name="line.50"></a>
<span class="sourceLineNo">051</span>   */<a name="line.51"></a>
<span class="sourceLineNo">052</span>  @InterfaceAudience.Private<a name="line.52"></a>
<span class="sourceLineNo">053</span>  ReplicationPeerConfigBuilder putConfiguration(String key, String value);<a name="line.53"></a>
<span class="sourceLineNo">054</span><a name="line.54"></a>
<span class="sourceLineNo">055</span>  /**<a name="line.55"></a>
<span class="sourceLineNo">056</span>   * Adds all of the provided "raw" configuration entries to {@code this}.<a name="line.56"></a>
<span class="sourceLineNo">057</span>   * @param configuration A collection of raw configuration entries<a name="line.57"></a>
<span class="sourceLineNo">058</span>   * @return {@code this}<a name="line.58"></a>
<span class="sourceLineNo">059</span>   */<a name="line.59"></a>
<span class="sourceLineNo">060</span>  @InterfaceAudience.Private<a name="line.60"></a>
<span class="sourceLineNo">061</span>  default ReplicationPeerConfigBuilder putAllConfiguration(Map&lt;String, String&gt; configuration) {<a name="line.61"></a>
<span class="sourceLineNo">062</span>    configuration.forEach(this::putConfiguration);<a name="line.62"></a>
<span class="sourceLineNo">063</span>    return this;<a name="line.63"></a>
<span class="sourceLineNo">064</span>  }<a name="line.64"></a>
<span class="sourceLineNo">065</span><a name="line.65"></a>
<span class="sourceLineNo">066</span>  /**<a name="line.66"></a>
<span class="sourceLineNo">067</span>   * Sets the serialized peer configuration data<a name="line.67"></a>
<span class="sourceLineNo">068</span>   * @return {@code this}<a name="line.68"></a>
<span class="sourceLineNo">069</span>   */<a name="line.69"></a>
<span class="sourceLineNo">070</span>  @InterfaceAudience.Private<a name="line.70"></a>
<span class="sourceLineNo">071</span>  ReplicationPeerConfigBuilder putPeerData(byte[] key, byte[] value);<a name="line.71"></a>
<span class="sourceLineNo">072</span><a name="line.72"></a>
<span class="sourceLineNo">073</span>  /**<a name="line.73"></a>
<span class="sourceLineNo">074</span>   * Sets all of the provided serialized peer configuration data.<a name="line.74"></a>
<span class="sourceLineNo">075</span>   * @return {@code this}<a name="line.75"></a>
<span class="sourceLineNo">076</span>   */<a name="line.76"></a>
<span class="sourceLineNo">077</span>  @InterfaceAudience.Private<a name="line.77"></a>
<span class="sourceLineNo">078</span>  default ReplicationPeerConfigBuilder putAllPeerData(Map&lt;byte[], byte[]&gt; peerData) {<a name="line.78"></a>
<span class="sourceLineNo">079</span>    peerData.forEach(this::putPeerData);<a name="line.79"></a>
<span class="sourceLineNo">080</span>    return this;<a name="line.80"></a>
<span class="sourceLineNo">081</span>  }<a name="line.81"></a>
<span class="sourceLineNo">082</span><a name="line.82"></a>
<span class="sourceLineNo">083</span>  /**<a name="line.83"></a>
<span class="sourceLineNo">084</span>   * Sets an explicit map of tables and column families in those tables that should be replicated<a name="line.84"></a>
<span class="sourceLineNo">085</span>   * to the given peer. Use {@link #setReplicateAllUserTables(boolean)} to replicate all tables<a name="line.85"></a>
<span class="sourceLineNo">086</span>   * to a peer.<a name="line.86"></a>
<span class="sourceLineNo">087</span>   *<a name="line.87"></a>
<span class="sourceLineNo">088</span>   * @param tableCFsMap A map from tableName to column family names. An empty collection can be<a name="line.88"></a>
<span class="sourceLineNo">089</span>   *    passed to indicate replicating all column families.<a name="line.89"></a>
<span class="sourceLineNo">090</span>   * @return {@code this}<a name="line.90"></a>
<span class="sourceLineNo">091</span>   * @see #setReplicateAllUserTables(boolean)<a name="line.91"></a>
<span class="sourceLineNo">092</span>   */<a name="line.92"></a>
<span class="sourceLineNo">093</span>  ReplicationPeerConfigBuilder<a name="line.93"></a>
<span class="sourceLineNo">094</span>      setTableCFsMap(Map&lt;TableName, List&lt;String&gt;&gt; tableCFsMap);<a name="line.94"></a>
<span class="sourceLineNo">095</span><a name="line.95"></a>
<span class="sourceLineNo">096</span>  /**<a name="line.96"></a>
<span class="sourceLineNo">097</span>   * Sets a unique collection of HBase namespaces that should be replicated to this peer.<a name="line.97"></a>
<span class="sourceLineNo">098</span>   * @param namespaces A set of namespaces to be replicated to this peer.<a name="line.98"></a>
<span class="sourceLineNo">099</span>   * @return {@code this}<a name="line.99"></a>
<span class="sourceLineNo">100</span>   */<a name="line.100"></a>
<span class="sourceLineNo">101</span>  ReplicationPeerConfigBuilder setNamespaces(Set&lt;String&gt; namespaces);<a name="line.101"></a>
<span class="sourceLineNo">102</span><a name="line.102"></a>
<span class="sourceLineNo">103</span>  /**<a name="line.103"></a>
<span class="sourceLineNo">104</span>   * Sets the speed, in bytes per second, for any one RegionServer to replicate data to the peer.<a name="line.104"></a>
<span class="sourceLineNo">105</span>   * @param bandwidth Bytes per second<a name="line.105"></a>
<span class="sourceLineNo">106</span>   * @return {@code this}.<a name="line.106"></a>
<span class="sourceLineNo">107</span>   */<a name="line.107"></a>
<span class="sourceLineNo">108</span>  ReplicationPeerConfigBuilder setBandwidth(long bandwidth);<a name="line.108"></a>
<span class="sourceLineNo">109</span><a name="line.109"></a>
<span class="sourceLineNo">110</span>  /**<a name="line.110"></a>
<span class="sourceLineNo">111</span>   * Configures HBase to replicate all user tables (not system tables) to the peer. Default is<a name="line.111"></a>
<span class="sourceLineNo">112</span>   * {@code true}.<a name="line.112"></a>
<span class="sourceLineNo">113</span>   * @param replicateAllUserTables True if all user tables should be replicated, else false.<a name="line.113"></a>
<span class="sourceLineNo">114</span>   * @return {@code this}<a name="line.114"></a>
<span class="sourceLineNo">115</span>   */<a name="line.115"></a>
<span class="sourceLineNo">116</span>  ReplicationPeerConfigBuilder setReplicateAllUserTables(boolean replicateAllUserTables);<a name="line.116"></a>
<span class="sourceLineNo">117</span><a name="line.117"></a>
<span class="sourceLineNo">118</span>  /**<a name="line.118"></a>
<span class="sourceLineNo">119</span>   * Sets the mapping of table name to column families which should not be replicated. This<a name="line.119"></a>
<span class="sourceLineNo">120</span>   * method sets state which is mutually exclusive to {@link #setTableCFsMap(Map)}. Invoking this<a name="line.120"></a>
<span class="sourceLineNo">121</span>   * method is only relevant when all user tables are being replicated.<a name="line.121"></a>
<span class="sourceLineNo">122</span>   *<a name="line.122"></a>
<span class="sourceLineNo">123</span>   * @param tableCFsMap A mapping of table names to column families which should not be<a name="line.123"></a>
<span class="sourceLineNo">124</span>   *    replicated. An empty list of column families implies all families for the table.<a name="line.124"></a>
<span class="sourceLineNo">125</span>   * @return {@code this}.<a name="line.125"></a>
<span class="sourceLineNo">126</span>   */<a name="line.126"></a>
<span class="sourceLineNo">127</span>  ReplicationPeerConfigBuilder setExcludeTableCFsMap(Map&lt;TableName, List&lt;String&gt;&gt; tableCFsMap);<a name="line.127"></a>
<span class="sourceLineNo">128</span><a name="line.128"></a>
<span class="sourceLineNo">129</span>  /**<a name="line.129"></a>
<span class="sourceLineNo">130</span>   * Sets the collection of namespaces which should not be replicated when all user tables are<a name="line.130"></a>
<span class="sourceLineNo">131</span>   * configured to be replicated. This method sets state which is mutually exclusive to<a name="line.131"></a>
<span class="sourceLineNo">132</span>   * {@link #setNamespaces(Set)}. Invoking this method is only relevant when all user tables are<a name="line.132"></a>
<span class="sourceLineNo">133</span>   * being replicated.<a name="line.133"></a>
<span class="sourceLineNo">134</span>   *<a name="line.134"></a>
<span class="sourceLineNo">135</span>   * @param namespaces A set of namespaces whose tables should not be replicated.<a name="line.135"></a>
<span class="sourceLineNo">136</span>   * @return {@code this}<a name="line.136"></a>
<span class="sourceLineNo">137</span>   */<a name="line.137"></a>
<span class="sourceLineNo">138</span>  ReplicationPeerConfigBuilder setExcludeNamespaces(Set&lt;String&gt; namespaces);<a name="line.138"></a>
<span class="sourceLineNo">139</span><a name="line.139"></a>
<span class="sourceLineNo">140</span>  /**<a name="line.140"></a>
<span class="sourceLineNo">141</span>   * &lt;p&gt;<a name="line.141"></a>
<span class="sourceLineNo">142</span>   * Sets whether we should preserve order when replicating, i.e, serial replication.<a name="line.142"></a>
<span class="sourceLineNo">143</span>   * &lt;/p&gt;<a name="line.143"></a>
<span class="sourceLineNo">144</span>   * &lt;p&gt;<a name="line.144"></a>
<span class="sourceLineNo">145</span>   * Default {@code false}.<a name="line.145"></a>
<span class="sourceLineNo">146</span>   * &lt;/p&gt;<a name="line.146"></a>
<span class="sourceLineNo">147</span>   * @param serial {@code true} means preserve order, otherwise {@code false}.<a name="line.147"></a>
<span class="sourceLineNo">148</span>   * @return {@code this}<a name="line.148"></a>
<span class="sourceLineNo">149</span>   */<a name="line.149"></a>
<span class="sourceLineNo">150</span>  ReplicationPeerConfigBuilder setSerial(boolean serial);<a name="line.150"></a>
<span class="sourceLineNo">151</span><a name="line.151"></a>
<span class="sourceLineNo">152</span>  /**<a name="line.152"></a>
<span class="sourceLineNo">153</span>   * Builds the configuration object from the current state of {@code this}.<a name="line.153"></a>
<span class="sourceLineNo">154</span>   * @return A {@link ReplicationPeerConfig} instance.<a name="line.154"></a>
<span class="sourceLineNo">155</span>   */<a name="line.155"></a>
<span class="sourceLineNo">156</span>  ReplicationPeerConfig build();<a name="line.156"></a>
<span class="sourceLineNo">157</span>}<a name="line.157"></a>




























































</pre>
</div>
</body>
</html>
