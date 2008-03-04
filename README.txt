$Id: README.txt,v 1.1 2008/03/04 23:54:46 alexj Exp $
Drupal revision_rss Overview
------------------------------------------------------------------------------
This module generates an RSS feed for node revisions. This allows users to 
subscribe to updates to individual nodes. This functionality is particularly 
useful for tracking changes to collaboratively developed content on auto-
revisioning node types.

This module allows administrators to customize which node types will have feeds 
generated for them, and which roles have permissions to view those feeds. The 
number of items per feed is taken from Drupal's internal RSS Publishing 
settings, but preferences about teasers and node full text are ignored; the 
revisions feed only contains node title, revision date and revision author.


Installation
------------------------------------------------------------------------------
  - Copy the module files to modules/
  - Enable the module on the Drupal admin page.


Credits / Contact
------------------------------------------------------------------------------
This module was created by Alex Jarvis (alexj[at]grrl.lib.mn.us), sponsored by 
the Great River Regional Library (http://www.griver.org).

Thanks to the developers of the Comment RSS (http://drupal.org/project/commentrss) module for inspiration.
