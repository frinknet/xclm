
Commands will follow the following naming conventions:

Terminology
-----------
Commands are comprised of the following components

 - noun = type of object to manipulate
 - nouns = identifies the type of id expected in printout
 - verb = manipulation to be preformed
 - verbed = identify that a manipulation was performed
 - adverbs = information nececary to manipulate
 - ids = one or more id of a noun


Setting information
-------------------

	noun-verb adverbs ids

prints one line of information per id
returns 1 on error otherwise 0


Testing Information
-------------------

	is-noun-verbed ids

prints nothing
returns 1 or 0 depending on check


List by Association
-------------------

	list-noun-nouns ids

ids are of the first noun type
prints ids of the second noun type
returns 1 on error otherwise 0


List by Status
--------------

	list-verbed-nouns

prints ids of the nouns affected
returns 1 on error otherwise 0



