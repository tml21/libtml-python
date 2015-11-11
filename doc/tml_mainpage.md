# TML Messaging Suite for Python {#mainpage}

## Introduction ##

**TML Messaging Suite** is a network messaging library for rapid development of extensible and scalable interfaces. Based on the peer to peer standard protocol [BEEP (Blocks Extensible Exchange Protocol)](http://www.beepcore.org), defined in [RFC3080](https://tools.ietf.org/html/rfc3080) and [RFC3081](https://tools.ietf.org/html/rfc3081) libTML is suitable for many use cases and communication patterns. Equipped with a type safe data API, TML can transport hierarchical data structures fast and reliable.

## Related Pages ##

  - [License](\ref tml_license)
  - [Installation](\ref tml_install)
  - [Introduction](\ref tml_intro)


## Library Documentation ##

### Low level API Documentation ###

The low level API is a python binding of the [C-API functions](http://libtml.org/docs/libtml-c-html/). 

  - \ref tmlGroup "TML: API reference"
  	- \ref coreGeneral "TML: General library functions"
  	- \ref coreHandle "TML: TMLCore functions"
  	- \ref tmlHandle "TML: Commands"
  - \ref sdxGroup "SIDEX: API reference"
  	- \ref sdxgeneral "SIDEX: General library functions"
  	- \ref sidexHandle "SIDEX: Document"
  	- \ref datatypes "SIDEX: Variant data type"

### libTML classes ###

To simplify the usage of libTML, the classes encapsulate the low level API. Reoccurring tasks like reference counting and type conversions are done automatically.

  - \ref TMLCoreGroup "TML Core: The TMLCore manages all communication of a peer."
  - \ref TMLProfileGroup "TML Profile: Representation of TML interfaces."
  - \ref TMLCmdGroup "TML Command: Contains status information and carries the data of a command or event call."
  - \ref TMLStreamGroup "TML Stream: Classes to send and receive streams with TML."
  - \ref SDXVariantGroup "SIDEX Variant: Data type conversion and handling of SIDEX variant data."
  - \ref SDXDocGroup "SIDEX Document: Access data in a SIDEX document."


## Contact us ##

For questions and community support you can send an email to the TML mailing list. 
To join the mailing list click on the link below.

[<b>Join the libTML mailing list</b>](https://groups.google.com/forum/#!forum/libtml/join "Join <b>libtml<b> mailing list")

Follow [<b>\@libtml</b>](https://twitter.com/libtml) on Twitter to get information about updates and news.

For professional support send an email 
to [<b>support@libtml.org</b>](mailto:support@libtml.org "libTML professionel support")
