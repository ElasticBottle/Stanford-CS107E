---
permalink: /about/
---
#### Course facts {{ site.data.quarter.name }}

  -   **Lectures:** {{ site.data.quarter.lectures }}
  -   **Labs**: {{ site.data.quarter.labs }}
  -   **Lecturers**: {{ site.data.quarter.instructors | map: "name" | join: ", " }}
  -   **CAs**: {{ site.data.quarter.cas | map: "name" | join: ", " }}
  -   **SLs**: {{ site.data.quarter.sls | map: "name" | join: ", " }}

{% include gallery.html %}

### About the course
CS107 is the third course in Stanford's introductory programming sequence.
The CS106A/B courses provide students with a solid foundation in programming methodology and abstractions, and CS107 follows on to develop the skills needed to build
computer systems. 

__CS107e: Computer Systems from the Ground Up__ is an experimental variant of CS107 that teaches the fundamental concepts of
computer systems through bare metal programming on the Raspberry Pi. Bare metal
programming means you will not run an operating system on the Raspberry Pi and
will make minimal use of libraries. This course also serves as an
introduction to embedded systems.The course will start with the microprocessor and move up to
the C programming language, without skipping anything in between. The goal is
to build a solid understanding of all aspects of how modern computers execute
programs and how program development tools work.

The major learning goals for the course are:

1. To understand how computers represent information, execute programs, and control peripherals.
2. To master command-line programming tools and the C programming language.

Topics covered include:

  -   the ARM architecture
  -   assembly language and machine-level code
  -   the C programming language
  -   compilation, linking and loading
  -   debugging
  -   memory organization and management
  -   controlling peripherals: GPIO, graphics, sound, and keyboards

Students will receive a Raspberry Pi and a kit of parts,
and all assignments will run on the Raspberry Pi.Assignments build upon each other by adding more and more functionality to a core library.
They culminate in a simple personal computer shell
using a keyboard and display. Finally, students do a project of their choosing 
where they build a complete hardware-software system.

This class is organized by weeks. Each week consists of two lectures
on Fri and Mon, a lab offered on Tue or Wed evening, and a programming
assignment which is due the following week. Lecture and lab attendance is mandatory. The final project is completed over the last two weeks of the quarter. There are no exams.

For information about the differences between CS107 and CS107e,
check out this [FAQ](https://web.stanford.edu/class/cs107e/).

