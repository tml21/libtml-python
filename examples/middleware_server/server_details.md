# A middleware server in Python #


### Defaults ###
- profileid: `urn:your-domain:com:Android01`
- ip listening: `0.0.0.0:4711`
- ip db-host: `127.0.0.1`
- db-user: `root`
- passwd: `bumblebee`
- db-name: `tmlexample`
- db-tablename: `items`
- db-charset: `utf-8`

#### Commands ####
- **cmd\_01\_load**: loads all saved items from the database and sends it back to the sender of the command.
- **cmd\_02\_insert**: adds a received item to the database.
- **cmd\_03\_update**: updates a specific item from the database with received informations.
- **cmd\_04\_delete**: deletes an item from database. 

#### During Init ####
A connection with a database is established. Then a TMLcore is initialised, a profile created and commands registered to the profile. 

