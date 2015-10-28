# Orion
Orion is a **NoSql database** entirely written in **C++** designed to manage large quantities of data located in
different servers, while being highly available with no single point of failure.<br />
Orion implements advanced memory pooling, shared memory, custom implementations for mutex, smart
pointers and different tree structures, including, **RedBlack** and **O2**.<br />
The key value memorization offers the possibility to executing queries with **thousands ORs** in an extremely
rapid way. Values corresponding to keys are organized in columns that, in turn, are clustered in column
families. Column families are defined when the database is created, but columns can be added to a family
anytime. Furthermore, columns are added by simply defining the keys and different keys can have a different
column number in the same family. Values belonging to a column family are memorized together, since Orion
uses a hybrid approach between column and row oriented DB.<br />
A particular **caching** and **sorting** system allows to efficiently manage queries with **several "IN" operators** and
thousands of elements and this makes Orion particularly suited to solve **geospatial queries** on moving parts and
complex tagging conditions.<br />
# Dependencies

# Examples

Start with example usage here: https://github.com/sommoyogurt/orion/tree/master/examples<br />