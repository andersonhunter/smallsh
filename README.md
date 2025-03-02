<head><strong>Welcome to smallsh!<strong></head><br><br>
smallsh is a Linux command shell in the style of bash, written in C.<br><br>
The syntax for running a command is<br><pre><code>command [arg1 arg2 ...] [&lt; input_file] [&gt; output_file] [&]</code></pre>where the bracketed items are optional depending on the command. The [&] arg specifies that the command will run in the background.<br><br>
The shell uses <strong>builtin commands</strong>, which <strong>do not</strong> fork a child and thus cannot be run in the background, and <strong>non-built in commands</strong> which do fork a child and thus <strong>can</strong> be run in the background.<br><br>
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

</tbody>
</table>