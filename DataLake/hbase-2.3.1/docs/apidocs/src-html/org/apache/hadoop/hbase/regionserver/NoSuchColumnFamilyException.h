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
<span class="sourceLineNo">019</span>package org.apache.hadoop.hbase.regionserver;<a name="line.19"></a>
<span class="sourceLineNo">020</span><a name="line.20"></a>
<span class="sourceLineNo">021</span>import org.apache.hadoop.hbase.DoNotRetryIOException;<a name="line.21"></a>
<span class="sourceLineNo">022</span>import org.apache.yetus.audience.InterfaceAudience;<a name="line.22"></a>
<span class="sourceLineNo">023</span><a name="line.23"></a>
<span class="sourceLineNo">024</span>/**<a name="line.24"></a>
<span class="sourceLineNo">025</span> * Thrown if request for nonexistent column family.<a name="line.25"></a>
<span class="sourceLineNo">026</span> */<a name="line.26"></a>
<span class="sourceLineNo">027</span>@InterfaceAudience.Public<a name="line.27"></a>
<span class="sourceLineNo">028</span>public class NoSuchColumnFamilyException extends DoNotRetryIOException {<a name="line.28"></a>
<span class="sourceLineNo">029</span>  private static final long serialVersionUID = -6569952730832331274L;<a name="line.29"></a>
<span class="sourceLineNo">030</span><a name="line.30"></a>
<span class="sourceLineNo">031</span>  /** default constructor */<a name="line.31"></a>
<span class="sourceLineNo">032</span>  public NoSuchColumnFamilyException() {<a name="line.32"></a>
<span class="sourceLineNo">033</span>    super();<a name="line.33"></a>
<span class="sourceLineNo">034</span>  }<a name="line.34"></a>
<span class="sourceLineNo">035</span><a name="line.35"></a>
<span class="sourceLineNo">036</span>  /**<a name="line.36"></a>
<span class="sourceLineNo">037</span>   * @param message exception message<a name="line.37"></a>
<span class="sourceLineNo">038</span>   */<a name="line.38"></a>
<span class="sourceLineNo">039</span>  public NoSuchColumnFamilyException(String message) {<a name="line.39"></a>
<span class="sourceLineNo">040</span>    super(message);<a name="line.40"></a>
<span class="sourceLineNo">041</span>  }<a name="line.41"></a>
<span class="sourceLineNo">042</span>}<a name="line.42"></a>




























































</pre>
</div>
</body>
</html>
