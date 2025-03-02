<head><strong>Welcome to smallsh!<strong></head><br><br>
smallsh is a Linux command shell in the style of bash, written in C.<br><br>
The syntax for running a command is<br><pre><code>command [arg1 arg2 ...] [&lt; input_file] [&gt; output_file] [&]</code></pre><br>where the bracketed items are optional depending on the command.<br><br>
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
<td align="left">Change current working directory.\nLeave blank for HOME dir, or specify path.&#13;&#10;Path can be relative or absolute.</td>
<td align="center">YES</td>
</table>