<div class="box">
	<h2>
		Next Generation Remote Display
	</h2>
	<p>
		NX is an exciting new technology for remote display. It provides <em>near local speed</em> application responsiveness over high latency, low bandwidth links. The core libraries for NX are provided by <a href="http://www.nomachine.com/">NoMachine</a> under the GPL. FreeNX is a GPL implementation of the NX Server.
	</p>
</div>
<div class="box">
	<h2>
		News
	</h2>
	<h3>
		<a
	href="http://mail.kde.org/pipermail/freenx-knx/2005-July/001690.html">FreeNX
	0.4.2 Released</a> 
        - Saturday, July 16, 2005 - 	
	[<a title="Download"
	href="https://developer.berlios.de/project/showfiles.php?group_id=2978&amp;release_id=6659">Download</a>]
	</h3>
	        <ul>
			<li> Removed forwarding support via "freenx.&lt;user&gt;", because it was buggy.</li>
			<li> Cleaned up lots of code.</li>
			<li> Fixed one case of a left over file.</li>
			<li> Removed one unnecessary usage of a temporary file.</li>
			<li> Added exit handlers in nxserver and nxnode for more stability on 
			     reboot of system. </li>
			<li> Changed nxnode to be much more stable.
			     <ul>
				<li> Catched one additional "unclean termination of nxagent" 
				     case, which lead to "zombie" sessions.
				</li>
			     </ul>
			</li>
			<li> Reworked reconnection support for more stability.
			     <ul>
				<li> Added displaying of reconnection failure in
				     system log / client ssh log.
				</li>
			     </ul>
			</li>
			<li> Added even more possibilities to catch and report
			     session startup failures.
			</li>
			<li> Fixed PNG and JPEG-levels pack method.</li>
			<li> Added usage of netcat -z to check that the port is really not used
			     by another agent.
			</li>
			<li> Added nxserver --cleanup, which terminates all running sessions.
			     Useful after a power outage.
			</li>
			<li> Fixed nxclient invocation with non-standard installation path </li>
			<li> Added detection of SSH2_CLIENT variable (commercial ssh) </li>
			<li> Finally fixed problems with GNOME. </li>
			<li> FINALLY added Solaris Support Patch. </li>
		</ul>
        <h3>
                <a href="http://mail.kde.org/pipermail/freenx-knx/2005-June/001416.html">FreeNX 0.4.1 Released</a> - Wednesday, June 29, 2005
        </h3>
                <ul>
                        <li>
                                Added support for 1.5.0 OSS components (especially rootless mode)
                        </li>
                        <li>
                                Added suspend/resume support for 1.5.0 OSS components
                        </li>
                </ul>
	<h3>
		<a href="http://mail.kde.org/pipermail/freenx-knx/2005-May/001240.html">FreeNX 0.4.0 Released</a> - Wednesday, May 4, 2005
	</h3>
		<ul>
			<li>
				Full filesharing support via Samba
			</li>
			<li>
				Sound support via ESD/artsd
			</li>
			<li>
				Printing support via Samba and a separate userspace cupsd for each user session
			</li>
		</ul>
</div>
