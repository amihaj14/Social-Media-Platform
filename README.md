# Social-Media-Platform
A raw social media platform that uses multiple data structures to store data.

## Overview


## Methodology
Different data structures were used for different needs.

_**User Tracking**_
An AVL tree is used as it places the users into it based on their unique user ID and it rebalances itself everytime a user is created and or deleted.

_**Friend System**_
An AVL tree is used here as well as a users friends are stored based on their user ID, thus the tree will balance itself once new friends are added or deleted.

_**Messaging System**_
An array was used for storing a users incoming and outgoing messages. This was the selected structure as the array allows for easy access to previous messages.

_**Posting System**_
Similarily to the last, multiple arrays were used in the creation of the posting function. This was done as the criteria was that each person will only see the 20 most recent posts, but the user themself has a post limit of 100. So two main arrays were used, one of size 20 and another of 100.
