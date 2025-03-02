<head>Welcome to smallsh!</head><br>
smallsh is a Linux command shell in the style of bash, written in C.<br>
The syntax for running a command is <pre><code>command [arg1 arg2 ...] [&lt; input_file] [&gt; output_file] [&]</code></pre> where the bracketed items are optional depending on the command.<br>
The shell uses <strong>builtin commands</strong>, which <strong>do not</strong> fork a child and thus cannot be run in the background, and <strong>non-built in commands</strong> which do fork a child and thus <strong>can</strong> be run in the background.<br>
Currently, smallsh supports the following commands:
<table>
<thead>
<tr>
<th>Command</th>
<th>Description</th>
<th>Builtin?</th>
</table>