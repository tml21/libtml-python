# Introduction to libTML-python {#tml_intro}

* [How to use the library](#howtouse)
* [Implementing the Interface](#firstprofile)
	* [Create and prepare a TML core and profile](#prepcoreprofile)
	* [Add command handler functions](#addcommand)
	* [Start the Listener](#startlistener)
* [Calling the Interface](#callinterface)
	* [Receiving progress replies](#recprogress)
* [Using the data property and variants](#dataproperty)
	* [Add Lists and Dictionaries](#listdictdata)


Many use cases are covered by the features of libTML. The example in this chapter is a brief overview of the most frequently used functions.

> All examples in this introduction contain only code lines to explain a special aspect of the library. Important things may be left out in favor of better readability. 

<a name="howtouse"></a>
## How to use the library ##

The libTML-python library provides access to the libtml-c low level functions and simplifies the usage with a set of Python objects. Two fundamental modules are available. TML (The Missing Link) provides the messaging and network functions and SIDEX (Simple Data Exchange) provides the API to access message data. After installing the library you can import TML and SIDEX into your project.

~~~~{.py}
import tml               # TML low level API
import tml.constants     # constant values
import tml.common        # helper functions
import tml.core          # TMLCore()
import tml.profile       # TMLProfile()
import tml.stream        # stream classes
import tml.command       # TMLCmd()

import sidex             # SIDEX low level API
import sidex.constants   # constant values
import sidex.variant     # SIDEX variant classes and type conversion
import sidex.document    # SDXDocument() 
~~~~

The following examples are using the name space in front of library calls to show which particular library is used. 
Using the low level API is similar to the C API in most aspects. The error handling is different in Python. Low level functions do not return an error code but raise an exception instead.
This introduction concentrates on the simplified usage of TML and SIDEX objects. The usage of low level functions is discussed more deeply in the [C API documentation](http://libtml.org/docs/libtml-c-html/). 

<a name="firstprofile"></a>
## Implementing the Interface ##

The TML communication is peer to peer (P2P) and the Client/Server pattern is a subset. The latter is used in this example to demonstrate how to implement and access a remote interface with TML.

<a name="prepcoreprofile"></a>
### Create and prepare a TML core and profile ###

To use TML at least one [TMLCore] object is required. To accept incoming traffic the listener thread needs an IP address, port and one or more profiles to be published.  A TML profile is the interface to refer to by a remote call.

~~~~{.py}
tmlcore = tml.core.TMLCore()
tmlcore.listener_binding = "127.0.0.1:12345"
~~~~
    
To identify a profile as a unique interface, a unique profile ID is required. Any string can be used, but usually a URN is selected. In this example `urn:your-domain:com:pyexample01` is used. The profile is referred several times and to avoid repeating, is assigned to `profileid` which is subsequently used.

~~~~{.py}
profileid = 'urn:your-domain:com:pyexample01'
~~~~

The profiles property of the [TMLCore] allows to access the interfaces assigned to it. It is an instance of [TMLProfiles]. To publish a profile it is added to the core.
    
~~~~{.py}
# register a command and profile
tmlcore.profiles.add_profile(profileid)
~~~~

<a name="addcommand"></a>
### Add command handler functions ###

A [profile.ON_COMMAND_CALL] function needs two parameters. A TML profile and a TML command. All handlers are implemented as seperate functions in this example but if an interface is implemented as an object, methods can be used as well. A [TML profile][TMLProfile] accepts methods as call handler and ignores the `self` parameter.

~~~~{.py}
# return a number and a unicode string
def cmd42handler(tmlprofile, cmd):
   cmd.data['Result']['Value'] = 42
   cmd.data['Result']['Test'] = "一切是伟大的"

# return a number and a list of values
def cmd43handler(tmlprofile, cmd):
   cmd.data['Result']['Value'] = 43
   cmd.data['Result']['Test'] = [1, 2, 3, 'test', 3.14]

# return a progress while executing the command
def cmd44_progress_test(tmlprofile, cmd):
   time.sleep(1)
   for p in range(0, 100, 10):
   		cmd.data['Result']['Progress'] = p
   		time.sleep(0.5)
   		cmd.progress = p
~~~~

To publish a [profile.ON_COMMAND_CALL] it is added to a profile using the register_cmd() method of the [TMLProfile] instance returned by the profiles property.

~~~~{.py}
# register commands
tmlcore.profiles[profileid].register_cmd(42, cmd42handler)
tmlcore.profiles[profileid].register_cmd(43, cmd43handler)
tmlcore.profiles[profileid].register_cmd(44, cmd44_progress_test)
~~~~

<a name="startlistener"></a>
### Start the Listener ###

Finally the listener is started to publish the interface to the network. TML uses multiple threads for parallel execution of the handler functions. Meanwhile the main thread needs to wait or can do different things. The `run()` function in this example is waiting for a keyboard input. In GUI programs it can easily be replaced with the application loop.

~~~~{.py}
tmlcore.listener_enabled = True
 
run() # wait until CTRL-C is pressed
~~~~

<a name="callinterface"></a>
## Calling the Interface ##

To call a particular interface a TCP/IP address and a profile ID is required. 

~~~~{.py}
profile = 'urn:your-domain:com:pyexample01'
port = '12345'
host = '127.0.0.1'
~~~~

The [TMLCore] is handling inbound and outbound messages. Before sending a command an instance of [TMLCore] is created. 

~~~~{.py}
core = tml.core.TMLCore()
~~~~

> Use the same [TMLCore] instance as long as the application is sending/receiving messages to keep the connections open. Reusing connections is much faster than starting a TCP/IP connection every time a message is sent.

Before sending a command, a [TMLCmd] object is created with the command ID of the remote interface function. It is sent with the [core.call_sync] method of [TMLCore]. Using synchronous calls with [core.call_sync] waits until a reply was received or an error occurs before returning. In case of an error an exception is raised.

~~~~{.py}
# call command 42
cmd = tml.command.TMLCmd(42)
core.call_sync(host, port, profile, cmd, 5000)
print(cmd.data['Result']['Value'])
print(cmd.data['Result']['Test'])

# call command 43
cmd = tml.command.TMLCmd(43)
core.call_sync(host, port, profile, cmd, 5000)
print(cmd.data['Result']['Value'])
print(cmd.data['Result']['Test'])
~~~~

<a name="recprogress"></a>
### Receiving progress replies ###

TML allows to send back status or progress replies to the caller. Besides providing information about the status of the command processing at the remote peer, progress replies reset the timeout passed to the [core.call_sync] or [core.call_async] method. In cases of long or unknown reply times, the receiver can keep the sender waiting.

The timeout reset is performed whether the progress reply is handled or not. Handling a progress reply is used to visualize the progress or any additional data.

The progress handler function can be assigned to a profile or, in this example, to the [TMLCore]. The object calling the progress handler is passed as the sender in the first parameter. The second parameter (cmd) is the command instance currently processed by the remote peer. The progress value is part of the command data, but it is passed for convenience as a third parameter.

~~~~{.py}
def progress_handler(sender, cmd, progress):
   print(progress)
~~~~

The progress handler has to be assigned to the [core][TMLCore] before sending the first command. Any subsequent call with progress replies will use the handler function as well.

~~~~{.py}
# call progress command
core.on_progress = progress_handler
cmd = tml.command.TMLCmd(44)
core.call_sync(host, port, profile, cmd, 5000)
~~~~

> If the data of the command is modified either at sender (progress handler) or receiver ([profile.ON_COMMAND_CALL]) side, the modifications are transferred together with the progress change. Applications can use this behavior to implement advanced features.

A script calling the commands of the interface in this example is producing the output below.

    42
    一切是伟大的
    43
    [1, 2, 3, 'test', 3.14]
    progress 10
    progress 20
    progress 30
    progress 40
    progress 50
    progress 60
    progress 70
    progress 80
    progress 90


<a name="dataproperty"></a>
## Using the data property and variants ##

The SIDEX API (Simple Data Exchange) provides powerful functions to organize data. Any [TMLCmd] instance has it's data stored in the [SDXDocument] available through the data property.

~~~~{.py}
# create a SDXDocument
sdoc = sidex.document.SDXDocument('SDXDocTest')
~~~~

An [SDXDocument] instance is storing data groups identified by a name (string). Each group can contain one or more values identified with keys (string). The python [SDXDocument] class mimics a dictionary to simplify data access.

~~~~{.py}
sdoc['General']['int'] = 42
sdoc['General']['float'] = 3.14
 
sdoc['General']['str'] = u"some text äüö !^s-§$"
sdoc['General']['chn'] = u"一切是伟大的"
sdoc['General']['bool'] = True
sdoc['General']['None'] = None
~~~~

All the above values are automatically converted into SIDEX variants and added to the document. 

The SIDEX variant types can be created and assigned like python values as well. The [SDXDateTime] example shows how to create a variant and assign it to the document. 

> Python date and time values can be converted automatically like other python values. The example with [SDXDateTime] is chosen for demonstration only.

~~~~{.py}
sdxval = sidex.variant.SDXDateTime("2014-01-01 12:30:00:000")
sdoc['General']['Date'] = sdxval
~~~~

Reading data from a [SDXDocument] is just using the dictionary behavior and address the value with group and key names.

~~~~{.py}
print(sdoc['General']['chn'])
~~~~

All values are automatically converted into Python values. If required set the [autocast](@ref sidex.document.SDXDocument.autocast) property of the [SDXDocument] to False to return all values as SDX variants.

<a name="listdictdata"></a>
### Add Lists and Dictionaries ###

Python container types like lists and dictionaries are a powerful tool to create hierarchical data structure. The [SDXDocument] supports lists and dictionaries as well and they can simply be added by assignment.

~~~~{.py}
sdoc['Container']['List'] = [1, 2, 3, u"一切是伟大的", None, False, sdxval,
                            [1, 2, 3], None, 
                            {'test': [1, sdxval, 3]}]
~~~~

The example shows the assignment of a list with an embedded list, a mixture of values including a SDX variant datetime (`sdxval`) and a dictionary with a list as value. 

[TMLCore]: @ref tml.core.TMLCore() "TMLCore"
[TMLProfiles]: @ref tml.profile.TMLProfiles() "TMLProfiles"
[TMLCmd]: @ref tml.command.TMLCmd() "TMLCmd"
[TMLProfile]: @ref tml.profile.TMLProfile() "TMLProfile"
[SDXDocument]: @ref sidex.document.SDXDocument "SDXDocument"
[core.call_sync]: @ref tml.core.TMLCore.call_sync() "call_sync()"
[core.call_async]: @ref tml.core.TMLCore.call_async() "call_async()"
[SDXDateTime]: @ref sidex.variant.SDXDateTime "SDXDateTime"
[profile.ON_COMMAND_CALL]: @ref _callback.ON_COMMAND_CALL() "command handler" 	








   




