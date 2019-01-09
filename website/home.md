---
permalink: /
title: Home
---

### Welcome to the {{ site.data.quarter.name }} offering of CS107e!

CS107 is the third course in Stanford's introductory programming sequence.
CS106 provides students 
with a solid foundation in programming methodology and abstractions,
and CS107 follows on to give you the skills needed to build computer
systems. The CS107E variant "Computer Systems from the Ground Up" specifically
focuses on bare metal programming on the Raspberry Pi.
Bare metal programming uses no operating system
and few external libraries.

There are two major learning goals for the course.

First, to understand how computers 
represent information, execute programs, and control peripherals.

Second, to master command-line programming tools
and the C programming language.

Students will receive a Raspberry Pi and a kit of parts,
and all assignments will run on the Raspberry Pi.
Assignments build upon each other
by adding more and more functionality to a core library.
They culminate in a simple personal computer shell
using a keyboard and display.

Finally, students do a project of their choosing 
where they build a complete hardware-software system.

For information about the differences between CS107 and CS107e,
check out this [FAQ](https://web.stanford.edu/class/cs107e/).


### Announcements

All class announcements will be posted on
[Piazza]({{ site.data.quarter.forum }}).

### Course Info

  -   **Lectures:** {{ site.data.quarter.lectures }}
  -   **Labs**: {{ site.data.quarter.labs }}
  -   **Lecturers**: {{ site.data.quarter.instructors | map: "name" | join: ", " }}
  -   **CAs**: {{ site.data.quarter.cas | map: "name" | join: ", " }}
  -   **SLs**: {{ site.data.quarter.sls | map: "name" | join: ", " }}

### Staff
{% include gallery.html %}


<!--
### Office Hours

<iframe src="https://calendar.google.com/calendar/embed?mode=WEEK&amp;height=400&amp;wkst=1&amp;bgcolor=%23FFFFFF&amp;src=2v2vsft4r6n68nd5n3hsiv0qjg%40group.calendar.google.com&amp;color=%235F6B02&amp;ctz=America%2FLos_Angeles" style="border-width:0" width="800" height="400" frameborder="0" scrolling="no"></iframe>
-->

### What's coming up next?
 {% include schedule.html current_only=true %}

View [schedule for entire quarter](/schedule/).
