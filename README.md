<head><b>--Welcome to <i>smallsh!</i>--</b></head><br><br>
<i>smallsh</i> is a Linux command shell in the style of bash, written in C.<br><br>
The syntax for running a command is
<br><pre><code>command [arg1 arg2 ...] [&lt; input_file] [&gt; output_file] [&]</code></pre>
where the bracketed items are optional depending on the command. The [&] arg specifies that the command will run in the background.<br><br>
The shell uses <b>builtin commands</b>, which <b>do not</b> fork a child and thus cannot be run in the background, and <b>non-built in commands</b> which do fork a child and thus <b>can</b> be run in the background.<br><br>
To run the shell, download the compiled binary, then, from your terminal of choice, cd to the directory containing smallsh and run the command <pre><code>./smallsh</pre></code><br>
The shell can toggle between foreground-only mode, wherein any non-built in command will ignore the [&] operator and only run in the foreground. The mode is toggled by sending the <i>SIGTSTP</i> signal, generally by using <i>CTRL+Z</i> on Windows (or, I assume, <i>CMD+Z</i> on Mac, but who can ever know).<br><br>
<i>smallsh</i> supports a number of basic commands, and I am adding more as time allows.<br><br>
Currently, smallsh supports the following commands:
<table>
<thead>
<tr>
<th align="center">Command</th>
<th align="center">Description</th>
<th align="center">Builtin?</th>
</thead>
<tbody>
<tr>
<td align="left">cd</td>
<td align="left">Change current working directory. Leave blank for HOME dir, or specify path. Path can be relative or absolute</td>
<td align="center">YES</td>
</tr>
<tr>
<td align="left">exit</td>
<td align="left">Perform cleanup by killing child processes, freeing dynamically-allocated memory, and exit shell</td>
<td align="center">YES</td>
</tr>
<tr>
<td align="left">pwd</td>
<td align="left">Print the current working directory to standard output, or specified output stream</td>
<td align="center">YES</td>
</tr>
<tr>
<td align="left">ls</td>
<td align="left">List items in current directory, or in directory specified by input directory, to standard output, or specified output stream</td>
<td align="center">YES</td>
</tr>
<tr>
<td align="left">status</td>
<td align="left">Write the status of the last-performed foreground process to standard output, or to the specified output stream</td>
<td align="center">YES</td>
</tr>
<tr>
<td align="left">wc</td>
<td align="left">Count words in specified input file, and print count to standard output, or to specified output stream</td>
<td align="center">NO</td>
</tr>
<tr>
<td align="left">cat</td>
<td align="left">Read specified input file to standard output, or to specified output stream</td>
<td align="center">NO</td>
</tr>
<tr>
<td align="left">sleep</td>
<td align="left">Thread sleeps for the number of user-specified seconds</td>
<td align="center">NO</td>
</tr>
<tr>
<td align="left">ps</td>
<td align="left">Write the names and PIDs of the processes running on the current environment to standard output, or specified output stream</td>
<td align="center">NO</td>
</tr>
<tr>
<td align="left">mkdir</td>
<td align="left">Create a directory with the specified name in the current directory</td>
<td align="center">NO</td>
</tr>
<tr>
<td align="left">test</td>
<td align="left">Determines if the specified arguments match, i.e. <pre><code>test ./fileName file</code></pre> determines if ./fileName is a file</td>
<td align="center">NO</td>
</tr>
<tr>
<td align="left">kill</td>
<td align="left">Kill the process whose PID is specified as an arg using the specified flags, defaulted to SIGTERM</td>
<td align="center">NO</td>
</tr>
<tr>
<td align="left">pkill</td>
<td align="left">Attempts to kill all currently-running process whose names match the name specified as an arg with the specified flag, defaulted to SIGTERM</td>
<td align="center">NO</td>
</tr>
<tr>
<td align="left">cat</td>
<td align="left">Read specified input file to standard output, or to specified output stream</td>
<td align="center">NO</td>
</tr>
</tbody>
</table><br><br>
Some fun facts:<br>
<ul>
<li><i>smallsh</i> monitors child processes with a linked list, <a href="https://youtu.be/-TMdron2JhY?si=aSIc3S467XUK1sJw">which we all know are not real</a></li>
<li><i>smallsh</i> can support up to 2048 characters of input, and 512 maximum arguments. (I'm not really sure why you'd need 512 args...)</li>
<li>I currently plan on customizing the shell a little more to make it unique, probably following some sort of media-related pun. The best idea I've had so far is based off <i>I Love Lucy</i>, using commands like <i>lucy</i> instead of <i>touch</i>, and [littleRicky] instead of [&] since the background commands spawn a child process</li>