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
	href="http://mail.kde.org/pipermail/freenx-knx/2007-January/004535.html">FreeNX
	0.6.0 Released</a> 
        - Tuesday, January 23, 2007 - 	
	[<a title="Download"
	href="http://prdownload.berlios.de/freenx/freenx-0.6.0.tar.gz">Download</a>]
	</h3>
	<pre>
23.01.2007 FreeNX 0.6.0 "Juliana birthday edition"
        * Opened the 0.6.0 branch.
        * Added nxnode slave mode.
        * General code cleanup.
                * Huge cleanup of nxnode.
        * Removed "no-x11-forwarding" from keys to allow client to use the
          faster interactive sessions.
        * Fixed nxsetup automatic testing of sessions and cleared up
          explanations.
        * Added support for NX 2.0.0 style nxclient dialogs.
        * Support for NX 2.0.0 backend in nxloadconfig.
        * Fixed cups printing (added username and password).
        * Fixed one more stray tail process.
        * Added example script for "load" based loadbalancing.
        * Fixed spaces in parameters for NX Client 2.0.0.
          (ssycplkbocve@spammotel.com)
        * Added version 2.1.0 support.
        * Fixed LD_LIBRARY_PRELOAD default path in nxloadconfig.
        * Fixed nxclient to work with 2.1.0 backend.
        * Added autodetection of backend.

01.07.2006 FreeNX 0.5.0 "UKUUG 2006 Edition"
        * Opened the 0.5.0 branch.
        * Added load balancing.
        * Completely removed support for 1.4.0 backend.
        * Rootless mode is now the default.
        * Reworked nxnode / suspend on connection failure should work now.
        * Added support for "Running" sessions.
        * Fixed --send command. (Emmanuel Blindauer )
        * Fixed resume with nxclient >=1.5.0-106 for Windows.
        * Fixed rootless sessions with Windows nxclient.
        * Fixed keyboard issues by enabling the keybd channel.
        * Fixed one more stray tail process and being able to
          cleanup after a hopelessly failed reconnection. (i.e. agent died)
        * Fixed detection of xauth / netcat. Added option to disable extra
          checks.
        * Fixed --terminate / --suspend when hostname has a '-' in it.
          (Emmanuel Blindauer )
        * ESD_NO_SPAWN is always set when ESPEAKER is set.
          ("Felipe Alfaro Solana" )
        * Added perl replacement for 'rev' function on Sun OS 5.10.
        * Fixed NODE_AUTOSTART to be unable to block sessions.
        * Fixed stale sessions introduced by the new session handling model.
        * Added usage of TCP NODELAY option.
        * Fixed loadbalancing IP issues.
        * Added --force-terminate to remove session info.
          Fixed issue with suspend/terminate commands.
        * Added correct errorcode 596 instead of 504.
        * Implemented "round-robin" and "load" loadbalancing algorithms.
          Cleaned up node.conf keys.
        * Fixed help for --restart.
        * Fixed session_running function, which fixes all remaining stale
          session problems.
        * Removed termination of nxagent in case of rootless mode.
          (Fixes kontact without --nofork)
        * Last minute fixes for new functions using rev.
        * Added experimental last minute support for NX 2.0.0 backend.
          (set ENABLE_2_0_0_BACKEND=1)

XX.XX.2006 FreeNX 0.4.5 "aKademy Edition"
        * Made nxsetup more user-friendly and hopefully finally failsafe.
        * Added --agent to nxnode/nxserver to allow easier debugging.
        * Added addgroup/groupadd to nxsetup
        * Added --ignore-errors support on nxsetup/nxloadconfig
        * Added check for expect.
	</pre>

	<h3>		<a
	href="http://wiki.debian.org/DebianEdu/DevCamp2006">FreeNX at Debian-Edu DevCamp 2006.</a> 
        - Saturday, July 08, 2006 -
	</h3>
	FreeNX is currently present at the Debian-Edu DevCamp 2006 in
	Forbach, France. You can visit us there.

	<h3>		<a
	href="http://www.ukuug.org/events/linux2006/">FreeNX at UKUUG 2006.</a> 
        - Saturday, July 01, 2006 -
	</h3>
	FreeNX is currently present at UKUUG Linux 2006 conference in
	Brighton, England. You can visit us there.

	<h3>		<a
	href="http://www.linuxjournal.com/article/8477">Visit FreeNX on Linux WorldExpo San Francisco Aug 9-11, 2005</a> 
        - Saturday, August 07, 2005 - 
	</h3>
	We also have a nice <a href="http://www.linuxjournal.com/article/8477">article series</a> at the moment, which prepares you for FreeNX during LWE.
	
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
