<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html lang="en">
<head>
<title>Source code</title>
<link rel="stylesheet" type="text/css" href="../../../../../../stylesheet.css" title="Style">
</head>
<body>
<div class="sourceContainer">
<pre><span class="sourceLineNo">001</span>/**<a name="line.1"></a>
<span class="sourceLineNo">002</span> *<a name="line.2"></a>
<span class="sourceLineNo">003</span> * Licensed to the Apache Software Foundation (ASF) under one<a name="line.3"></a>
<span class="sourceLineNo">004</span> * or more contributor license agreements.  See the NOTICE file<a name="line.4"></a>
<span class="sourceLineNo">005</span> * distributed with this work for additional information<a name="line.5"></a>
<span class="sourceLineNo">006</span> * regarding copyright ownership.  The ASF licenses this file<a name="line.6"></a>
<span class="sourceLineNo">007</span> * to you under the Apache License, Version 2.0 (the<a name="line.7"></a>
<span class="sourceLineNo">008</span> * "License"); you may not use this file except in compliance<a name="line.8"></a>
<span class="sourceLineNo">009</span> * with the License.  You may obtain a copy of the License at<a name="line.9"></a>
<span class="sourceLineNo">010</span> *<a name="line.10"></a>
<span class="sourceLineNo">011</span> *     http://www.apache.org/licenses/LICENSE-2.0<a name="line.11"></a>
<span class="sourceLineNo">012</span> *<a name="line.12"></a>
<span class="sourceLineNo">013</span> * Unless required by applicable law or agreed to in writing, software<a name="line.13"></a>
<span class="sourceLineNo">014</span> * distributed under the License is distributed on an "AS IS" BASIS,<a name="line.14"></a>
<span class="sourceLineNo">015</span> * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.<a name="line.15"></a>
<span class="sourceLineNo">016</span> * See the License for the specific language governing permissions and<a name="line.16"></a>
<span class="sourceLineNo">017</span> * limitations under the License.<a name="line.17"></a>
<span class="sourceLineNo">018</span> */<a name="line.18"></a>
<span class="sourceLineNo">019</span>package org.apache.hadoop.hbase.client;<a name="line.19"></a>
<span class="sourceLineNo">020</span><a name="line.20"></a>
<span class="sourceLineNo">021</span>import java.io.Closeable;<a name="line.21"></a>
<span class="sourceLineNo">022</span>import java.io.IOException;<a name="line.22"></a>
<span class="sourceLineNo">023</span>import java.util.List;<a name="line.23"></a>
<span class="sourceLineNo">024</span>import org.apache.hadoop.conf.Configuration;<a name="line.24"></a>
<span class="sourceLineNo">025</span>import org.apache.hadoop.hbase.TableName;<a name="line.25"></a>
<span class="sourceLineNo">026</span>import org.apache.yetus.audience.InterfaceAudience;<a name="line.26"></a>
<span class="sourceLineNo">027</span><a name="line.27"></a>
<span class="sourceLineNo">028</span>/**<a name="line.28"></a>
<span class="sourceLineNo">029</span> * &lt;p&gt;Used to communicate with a single HBase table similar to {@link Table} but meant for<a name="line.29"></a>
<span class="sourceLineNo">030</span> * batched, asynchronous puts. Obtain an instance from a {@link Connection} and call<a name="line.30"></a>
<span class="sourceLineNo">031</span> * {@link #close()} afterwards. Customizations can be applied to the {@code BufferedMutator} via<a name="line.31"></a>
<span class="sourceLineNo">032</span> * the {@link BufferedMutatorParams}.<a name="line.32"></a>
<span class="sourceLineNo">033</span> * &lt;/p&gt;<a name="line.33"></a>
<span class="sourceLineNo">034</span> *<a name="line.34"></a>
<span class="sourceLineNo">035</span> * &lt;p&gt;Exception handling with asynchronously via the {@link BufferedMutator.ExceptionListener}.<a name="line.35"></a>
<span class="sourceLineNo">036</span> * The default implementation is to throw the exception upon receipt. This behavior can be<a name="line.36"></a>
<span class="sourceLineNo">037</span> * overridden with a custom implementation, provided as a parameter with<a name="line.37"></a>
<span class="sourceLineNo">038</span> * {@link BufferedMutatorParams#listener(BufferedMutator.ExceptionListener)}.&lt;/p&gt;<a name="line.38"></a>
<span class="sourceLineNo">039</span> *<a name="line.39"></a>
<span class="sourceLineNo">040</span> * &lt;p&gt;Map/Reduce jobs are good use cases for using {@code BufferedMutator}. Map/reduce jobs<a name="line.40"></a>
<span class="sourceLineNo">041</span> * benefit from batching, but have no natural flush point. {@code BufferedMutator} receives the<a name="line.41"></a>
<span class="sourceLineNo">042</span> * puts from the M/R job and will batch puts based on some heuristic, such as the accumulated size<a name="line.42"></a>
<span class="sourceLineNo">043</span> * of the puts, and submit batches of puts asynchronously so that the M/R logic can continue<a name="line.43"></a>
<span class="sourceLineNo">044</span> * without interruption.<a name="line.44"></a>
<span class="sourceLineNo">045</span> * &lt;/p&gt;<a name="line.45"></a>
<span class="sourceLineNo">046</span> *<a name="line.46"></a>
<span class="sourceLineNo">047</span> * &lt;p&gt;{@code BufferedMutator} can also be used on more exotic circumstances. Map/Reduce batch jobs<a name="line.47"></a>
<span class="sourceLineNo">048</span> * will have a single {@code BufferedMutator} per thread. A single {@code BufferedMutator} can<a name="line.48"></a>
<span class="sourceLineNo">049</span> * also be effectively used in high volume online systems to batch puts, with the caveat that<a name="line.49"></a>
<span class="sourceLineNo">050</span> * extreme circumstances, such as JVM or machine failure, may cause some data loss.&lt;/p&gt;<a name="line.50"></a>
<span class="sourceLineNo">051</span> *<a name="line.51"></a>
<span class="sourceLineNo">052</span> * &lt;p&gt;NOTE: This class replaces the functionality that used to be available via<a name="line.52"></a>
<span class="sourceLineNo">053</span> * HTable#setAutoFlush(boolean) set to {@code false}.<a name="line.53"></a>
<span class="sourceLineNo">054</span> * &lt;/p&gt;<a name="line.54"></a>
<span class="sourceLineNo">055</span> *<a name="line.55"></a>
<span class="sourceLineNo">056</span> * &lt;p&gt;See also the {@code BufferedMutatorExample} in the hbase-examples module.&lt;/p&gt;<a name="line.56"></a>
<span class="sourceLineNo">057</span> * @see ConnectionFactory<a name="line.57"></a>
<span class="sourceLineNo">058</span> * @see Connection<a name="line.58"></a>
<span class="sourceLineNo">059</span> * @since 1.0.0<a name="line.59"></a>
<span class="sourceLineNo">060</span> */<a name="line.60"></a>
<span class="sourceLineNo">061</span>@InterfaceAudience.Public<a name="line.61"></a>
<span class="sourceLineNo">062</span>public interface BufferedMutator extends Closeable {<a name="line.62"></a>
<span class="sourceLineNo">063</span>  /**<a name="line.63"></a>
<span class="sourceLineNo">064</span>   * Key to use setting non-default BufferedMutator implementation in Configuration.<a name="line.64"></a>
<span class="sourceLineNo">065</span>   */<a name="line.65"></a>
<span class="sourceLineNo">066</span>  String CLASSNAME_KEY = "hbase.client.bufferedmutator.classname";<a name="line.66"></a>
<span class="sourceLineNo">067</span><a name="line.67"></a>
<span class="sourceLineNo">068</span>  /**<a name="line.68"></a>
<span class="sourceLineNo">069</span>   * Having the timer tick run more often that once every 100ms is needless and will<a name="line.69"></a>
<span class="sourceLineNo">070</span>   * probably cause too many timer events firing having a negative impact on performance.<a name="line.70"></a>
<span class="sourceLineNo">071</span>   */<a name="line.71"></a>
<span class="sourceLineNo">072</span>  long MIN_WRITE_BUFFER_PERIODIC_FLUSH_TIMERTICK_MS = 100;<a name="line.72"></a>
<span class="sourceLineNo">073</span><a name="line.73"></a>
<span class="sourceLineNo">074</span>  /**<a name="line.74"></a>
<span class="sourceLineNo">075</span>   * Gets the fully qualified table name instance of the table that this BufferedMutator writes to.<a name="line.75"></a>
<span class="sourceLineNo">076</span>   */<a name="line.76"></a>
<span class="sourceLineNo">077</span>  TableName getName();<a name="line.77"></a>
<span class="sourceLineNo">078</span><a name="line.78"></a>
<span class="sourceLineNo">079</span>  /**<a name="line.79"></a>
<span class="sourceLineNo">080</span>   * Returns the {@link org.apache.hadoop.conf.Configuration} object used by this instance.<a name="line.80"></a>
<span class="sourceLineNo">081</span>   * &lt;p&gt;<a name="line.81"></a>
<span class="sourceLineNo">082</span>   * The reference returned is not a copy, so any change made to it will<a name="line.82"></a>
<span class="sourceLineNo">083</span>   * affect this instance.<a name="line.83"></a>
<span class="sourceLineNo">084</span>   */<a name="line.84"></a>
<span class="sourceLineNo">085</span>  Configuration getConfiguration();<a name="line.85"></a>
<span class="sourceLineNo">086</span><a name="line.86"></a>
<span class="sourceLineNo">087</span>  /**<a name="line.87"></a>
<span class="sourceLineNo">088</span>   * Sends a {@link Mutation} to the table. The mutations will be buffered and sent over the<a name="line.88"></a>
<span class="sourceLineNo">089</span>   * wire as part of a batch. Currently only supports {@link Put} and {@link Delete} mutations.<a name="line.89"></a>
<span class="sourceLineNo">090</span>   *<a name="line.90"></a>
<span class="sourceLineNo">091</span>   * @param mutation The data to send.<a name="line.91"></a>
<span class="sourceLineNo">092</span>   * @throws IOException if a remote or network exception occurs.<a name="line.92"></a>
<span class="sourceLineNo">093</span>   */<a name="line.93"></a>
<span class="sourceLineNo">094</span>  void mutate(Mutation mutation) throws IOException;<a name="line.94"></a>
<span class="sourceLineNo">095</span><a name="line.95"></a>
<span class="sourceLineNo">096</span>  /**<a name="line.96"></a>
<span class="sourceLineNo">097</span>   * Send some {@link Mutation}s to the table. The mutations will be buffered and sent over the<a name="line.97"></a>
<span class="sourceLineNo">098</span>   * wire as part of a batch. There is no guarantee of sending entire content of {@code mutations}<a name="line.98"></a>
<span class="sourceLineNo">099</span>   * in a single batch; it will be broken up according to the write buffer capacity.<a name="line.99"></a>
<span class="sourceLineNo">100</span>   *<a name="line.100"></a>
<span class="sourceLineNo">101</span>   * @param mutations The data to send.<a name="line.101"></a>
<span class="sourceLineNo">102</span>   * @throws IOException if a remote or network exception occurs.<a name="line.102"></a>
<span class="sourceLineNo">103</span>   */<a name="line.103"></a>
<span class="sourceLineNo">104</span>  void mutate(List&lt;? extends Mutation&gt; mutations) throws IOException;<a name="line.104"></a>
<span class="sourceLineNo">105</span><a name="line.105"></a>
<span class="sourceLineNo">106</span>  /**<a name="line.106"></a>
<span class="sourceLineNo">107</span>   * Performs a {@link #flush()} and releases any resources held.<a name="line.107"></a>
<span class="sourceLineNo">108</span>   *<a name="line.108"></a>
<span class="sourceLineNo">109</span>   * @throws IOException if a remote or network exception occurs.<a name="line.109"></a>
<span class="sourceLineNo">110</span>   */<a name="line.110"></a>
<span class="sourceLineNo">111</span>  @Override<a name="line.111"></a>
<span class="sourceLineNo">112</span>  void close() throws IOException;<a name="line.112"></a>
<span class="sourceLineNo">113</span><a name="line.113"></a>
<span class="sourceLineNo">114</span>  /**<a name="line.114"></a>
<span class="sourceLineNo">115</span>   * Executes all the buffered, asynchronous {@link Mutation} operations and waits until they<a name="line.115"></a>
<span class="sourceLineNo">116</span>   * are done.<a name="line.116"></a>
<span class="sourceLineNo">117</span>   *<a name="line.117"></a>
<span class="sourceLineNo">118</span>   * @throws IOException if a remote or network exception occurs.<a name="line.118"></a>
<span class="sourceLineNo">119</span>   */<a name="line.119"></a>
<span class="sourceLineNo">120</span>  void flush() throws IOException;<a name="line.120"></a>
<span class="sourceLineNo">121</span><a name="line.121"></a>
<span class="sourceLineNo">122</span>  /**<a name="line.122"></a>
<span class="sourceLineNo">123</span>   * Sets the maximum time before the buffer is automatically flushed checking once per second.<a name="line.123"></a>
<span class="sourceLineNo">124</span>   * @param timeoutMs    The maximum number of milliseconds how long records may be buffered<a name="line.124"></a>
<span class="sourceLineNo">125</span>   *                     before they are flushed. Set to 0 to disable.<a name="line.125"></a>
<span class="sourceLineNo">126</span>   */<a name="line.126"></a>
<span class="sourceLineNo">127</span>  default void setWriteBufferPeriodicFlush(long timeoutMs) {<a name="line.127"></a>
<span class="sourceLineNo">128</span>    setWriteBufferPeriodicFlush(timeoutMs, 1000L);<a name="line.128"></a>
<span class="sourceLineNo">129</span>  }<a name="line.129"></a>
<span class="sourceLineNo">130</span><a name="line.130"></a>
<span class="sourceLineNo">131</span>  /**<a name="line.131"></a>
<span class="sourceLineNo">132</span>   * Sets the maximum time before the buffer is automatically flushed.<a name="line.132"></a>
<span class="sourceLineNo">133</span>   * @param timeoutMs    The maximum number of milliseconds how long records may be buffered<a name="line.133"></a>
<span class="sourceLineNo">134</span>   *                     before they are flushed. Set to 0 to disable.<a name="line.134"></a>
<span class="sourceLineNo">135</span>   * @param timerTickMs  The number of milliseconds between each check if the<a name="line.135"></a>
<span class="sourceLineNo">136</span>   *                     timeout has been exceeded. Must be 100ms (as defined in<a name="line.136"></a>
<span class="sourceLineNo">137</span>   *                     {@link #MIN_WRITE_BUFFER_PERIODIC_FLUSH_TIMERTICK_MS})<a name="line.137"></a>
<span class="sourceLineNo">138</span>   *                     or larger to avoid performance problems.<a name="line.138"></a>
<span class="sourceLineNo">139</span>   */<a name="line.139"></a>
<span class="sourceLineNo">140</span>  default void setWriteBufferPeriodicFlush(long timeoutMs, long timerTickMs) {<a name="line.140"></a>
<span class="sourceLineNo">141</span>    throw new UnsupportedOperationException(<a name="line.141"></a>
<span class="sourceLineNo">142</span>            "The BufferedMutator::setWriteBufferPeriodicFlush has not been implemented");<a name="line.142"></a>
<span class="sourceLineNo">143</span>  }<a name="line.143"></a>
<span class="sourceLineNo">144</span><a name="line.144"></a>
<span class="sourceLineNo">145</span>  /**<a name="line.145"></a>
<span class="sourceLineNo">146</span>   * Disable periodic flushing of the write buffer.<a name="line.146"></a>
<span class="sourceLineNo">147</span>   */<a name="line.147"></a>
<span class="sourceLineNo">148</span>  default void disableWriteBufferPeriodicFlush() {<a name="line.148"></a>
<span class="sourceLineNo">149</span>    setWriteBufferPeriodicFlush(0, MIN_WRITE_BUFFER_PERIODIC_FLUSH_TIMERTICK_MS);<a name="line.149"></a>
<span class="sourceLineNo">150</span>  }<a name="line.150"></a>
<span class="sourceLineNo">151</span><a name="line.151"></a>
<span class="sourceLineNo">152</span>  /**<a name="line.152"></a>
<span class="sourceLineNo">153</span>   * Returns the current periodic flush timeout value in milliseconds.<a name="line.153"></a>
<span class="sourceLineNo">154</span>   * @return The maximum number of milliseconds how long records may be buffered before they<a name="line.154"></a>
<span class="sourceLineNo">155</span>   *   are flushed. The value 0 means this is disabled.<a name="line.155"></a>
<span class="sourceLineNo">156</span>   */<a name="line.156"></a>
<span class="sourceLineNo">157</span>  default long getWriteBufferPeriodicFlushTimeoutMs() {<a name="line.157"></a>
<span class="sourceLineNo">158</span>    throw new UnsupportedOperationException(<a name="line.158"></a>
<span class="sourceLineNo">159</span>            "The BufferedMutator::getWriteBufferPeriodicFlushTimeoutMs has not been implemented");<a name="line.159"></a>
<span class="sourceLineNo">160</span>  }<a name="line.160"></a>
<span class="sourceLineNo">161</span><a name="line.161"></a>
<span class="sourceLineNo">162</span>  /**<a name="line.162"></a>
<span class="sourceLineNo">163</span>   * Returns the current periodic flush timertick interval in milliseconds.<a name="line.163"></a>
<span class="sourceLineNo">164</span>   * @return The number of milliseconds between each check if the timeout has been exceeded.<a name="line.164"></a>
<span class="sourceLineNo">165</span>   *   This value only has a real meaning if the timeout has been set to &gt; 0<a name="line.165"></a>
<span class="sourceLineNo">166</span>   */<a name="line.166"></a>
<span class="sourceLineNo">167</span>  default long getWriteBufferPeriodicFlushTimerTickMs() {<a name="line.167"></a>
<span class="sourceLineNo">168</span>    throw new UnsupportedOperationException(<a name="line.168"></a>
<span class="sourceLineNo">169</span>            "The BufferedMutator::getWriteBufferPeriodicFlushTimerTickMs has not been implemented");<a name="line.169"></a>
<span class="sourceLineNo">170</span>  }<a name="line.170"></a>
<span class="sourceLineNo">171</span><a name="line.171"></a>
<span class="sourceLineNo">172</span>  /**<a name="line.172"></a>
<span class="sourceLineNo">173</span>   * Returns the maximum size in bytes of the write buffer for this HTable.<a name="line.173"></a>
<span class="sourceLineNo">174</span>   * &lt;p&gt;<a name="line.174"></a>
<span class="sourceLineNo">175</span>   * The default value comes from the configuration parameter {@code hbase.client.write.buffer}.<a name="line.175"></a>
<span class="sourceLineNo">176</span>   * @return The size of the write buffer in bytes.<a name="line.176"></a>
<span class="sourceLineNo">177</span>   */<a name="line.177"></a>
<span class="sourceLineNo">178</span>  long getWriteBufferSize();<a name="line.178"></a>
<span class="sourceLineNo">179</span><a name="line.179"></a>
<span class="sourceLineNo">180</span>  /**<a name="line.180"></a>
<span class="sourceLineNo">181</span>   * Set rpc timeout for this mutator instance<a name="line.181"></a>
<span class="sourceLineNo">182</span>   */<a name="line.182"></a>
<span class="sourceLineNo">183</span>  void setRpcTimeout(int timeout);<a name="line.183"></a>
<span class="sourceLineNo">184</span><a name="line.184"></a>
<span class="sourceLineNo">185</span>  /**<a name="line.185"></a>
<span class="sourceLineNo">186</span>   * Set operation timeout for this mutator instance<a name="line.186"></a>
<span class="sourceLineNo">187</span>   */<a name="line.187"></a>
<span class="sourceLineNo">188</span>  void setOperationTimeout(int timeout);<a name="line.188"></a>
<span class="sourceLineNo">189</span><a name="line.189"></a>
<span class="sourceLineNo">190</span>  /**<a name="line.190"></a>
<span class="sourceLineNo">191</span>   * Listens for asynchronous exceptions on a {@link BufferedMutator}.<a name="line.191"></a>
<span class="sourceLineNo">192</span>   */<a name="line.192"></a>
<span class="sourceLineNo">193</span>  @InterfaceAudience.Public<a name="line.193"></a>
<span class="sourceLineNo">194</span>  interface ExceptionListener {<a name="line.194"></a>
<span class="sourceLineNo">195</span>    public void onException(RetriesExhaustedWithDetailsException exception,<a name="line.195"></a>
<span class="sourceLineNo">196</span>        BufferedMutator mutator) throws RetriesExhaustedWithDetailsException;<a name="line.196"></a>
<span class="sourceLineNo">197</span>  }<a name="line.197"></a>
<span class="sourceLineNo">198</span>}<a name="line.198"></a>




























































</pre>
</div>
</body>
</html>
