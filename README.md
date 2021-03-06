CpScript
==================================


ANSI C
----------------------------------

<table>
  <tr>
    <th>Project</th><th>Description</th><th>Notes</th>
  </tr>
  <tr>
    <td>libCpScript.Asm</td>
	<td>C CpScript Assembly Library (Core CpScript)</td>
	<td>
	  <ul>
	    <li>Static and Shared Libs</li>
		<li>CpScript Standard Library</li>
	  </ul>
	</td>
  </tr>
  <tr>
    <td>libCpScript.ObjectBasic</td>
	<td>C CpScript Object Basic Library</td>
	<td>
	  <ul>
	    <li>Contains Both Static and Shared Libs</li>
		<li>Generates CpScript Assembly</li>
	  </ul>
    </td>
  </tr>
  <tr>
    <td>cpInt</td>
	<td>CpScript Assembly Runtime</td>
	<td>
	  <ul>
	    <li>Runtime Environment For Cp Script Assembly</li>
		<li>Loads and Runs CpScript Assembly and CpScript Binary Script</li>
		<li>Can Compile CpScript Assembly Into CpScript Binary</li>
	  </ul>
    </td>
  </tr>
  <tr>
    <td>cpObjCmp</td>
	<td>Compiles CpScript Object Basic into CpScript Assembly</td>
	<td></td>
  </tr>
  <tr>
    <td>cpConsole</td>
	<td>Interactive CpScript Object Basic Console</td>
	<td></td>
  </tr>
  <tr>
    <td>execRuntime</td>
	<td>Runtime base app to create standalone Object Basic Script executables</td>
	<td></td>
  </tr>
  <tr>
    <td>makeExec</td>
	<td>Tool to compile Object Basic Script into standalone executable.</td>
	<td></td>
  </tr>
</table>
	
.NET
----------------------------------

<table>
  <tr>
    <th>Project</th><th>Description</th><th>Notes</th>
  </tr>
  <tr>
    <td>libCpScript.Net.StdLib</td>
	<td>C# Interop Library For libCpStdLib</td>
	<td>
	  <ul>
	    <li>P/Invoke</li>
	  </ul>
	</td>
  </tr>
  <tr>
    <td>libCpScript.Net.Asm</td>
	<td>C# Interop Library For libCpScript.Asm</td>
	<td>
	  <ul>
	    <li>P/Invoke</li>
	  </ul>
	</td>
  </tr>
  <tr>
    <td>libCpScript.Net.ObjectBasic</td>
	<td>C# Interop Library For libCpScript.ObjectBasic & libCpScript.Asm</td>
	<td>
	  <ul>
	    <li>P/Invoke</li>
	  </ul>
	</td>
  </tr>
</table>	

Building
==================================

<ul>
	<li>.NET
		<ul>
			<li>32Bit .NET dll's are pre-built, and under the Build directory</li>
			<li>A MonoDevelop solution is available under the CSharp folder if you want to rebuild the .NET libraries.</li>
		</ul>
	</li>
	<li>ANSI C
		<ul>
			<li>Requirements
				<ul>
					<li>Windows - MingW32 Build Environment OR Cygwin</li>
					<li>Linux - Standard Build Environment</li>
				</ul>
			</li>
			<li>Building
				<ul>
					<li>Enter the C directory</li>
					<li>Windows - mingw32-make -f Makefile.win</li>
					<li>Linux or Cygwin - make</li>
				</ul>
			</li>
			<li>All files are compiled under the Build directory from the project root directory</li>
		</ul>
	</li>
</ul>
  
	
