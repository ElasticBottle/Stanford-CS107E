---
released: true
permalink: /assignments/project/
title: 'Final Project'
toc: true
---
{% comment %}
Task list to copy/paste when creating PR:

__Before releasing project:__
- [ ] Review writeup/starter code (instructor)
- [ ] Consider changes for clarity of spec/ease of grading (TA)
- [ ] Followup on any github issue from previous

__To prep for project:__
- [ ] 

{% endcomment %}

The final assignment is a project of your own choosing. 
The goal of the project is to consolidate 
your knowledge of the key concepts covered in the CS107e course,
and to do something creative with your Raspberry Pi. We hope you find the project a fun and satisfying way to put your new knowledge and skills into action and build something you are truly proud of!


### Project schedule and deliverables

You have over two weeks to complete the project. 
Please take note of these important milestones and deliverables in the project schedule:


- #### Team formation due __6pm Mon Mar 4__

    Form a team of 1 to 3 people. We've observed that a 2-person team works out the best, so that is our recommendation. If you have a project in mind that would be better run as a solo effort or requires a team of 3, that's ok too, but you may want to talk it through with us to understand the tradeoffs and challenges for the different arrangements.

    Submit the Github usernames of your team members on this 
    [Google form](https://goo.gl/forms/UIuzeX0hZPrxWpdw1).
    We will create a shared repository for your team.

- #### Project lab 1

    Labs will meet twice during the final project period. These labs will be used as a working meeting for your team. You and your teammates should arrange to attend the same lab session.

    In the first project lab, you will brainstorm project options and refine those ideas into a specific project. Your team will then draft a plan submit as your project proposal.

- #### Proposal due __6pm Fri Mar 8__

   You will receive an email from GitHub when your team's repository is
 created. Clone this repository and edit the file `Proposal.md` file to add your
   project information. Commit and push to Github to submit the proposal.

   The project proposal should include:

   - Name of the project
   - Description of project and overall end goal
   - Team members and their responsibilities
       + Each team member should be responsible for a clearly
   identifiable part of the project.
   - Midpoint milestones (1 week left to finish)
       + These are the goals we will review with you at the checkup during the second project lab
   - Resources needed

   Each project team has a budget of $20 per person for hardware. Save your receipts to be reimbursed.
   If you need to mail order parts, it can take time,
   so figure out what you need ASAP and place the order.
   Also, you should first check the list of sensors we already have 
   on hand.

- #### Project lab 2

   In the second project lab, we will review your progress on meeting your midpoint milestones and ensure you are on-track for completing the remaining work.

   Please take advantage of this check-in with the course staff; we are eager for you to succeed with your project and want to help resolve any snags you encounter.


- #### Demo session is __9-11:30am Fri Mar 22__

   We use our scheduled final exam slot as the demonstration session for the final projects. Each group should prepare a short project demo to deliver. This is a fun event where you get to show off your work and see all the cool things done by your peers. There is much to celebrate!

- #### Final submission due __9am Fri Mar 22__

   You should be using the github repository for coordinating the
   work amongst your team throughout the project.
   As such, you should be regularly checking in and out code.
   The final commit of your code must be received by the morning of the demo session.

   As part of your final submission, edit the `README.md` file of your repository to give a writeup about your completed project.  The writeup should include a short description of what work was performed by each member of the team.

   If your project was influenced by ideas or work of other people,
   please attribute those contributions in the writeup.
   In particular, if incorporating code from others, you must reference where you got the code,
   and what changes you made.
   It should be clear what you did, and what others did. Your project will be primarily evaluated on the portion of the work that is original.


### Additional resources

In addition to going further with the components you have used previously in the course, you are also welcome to explore additional hardware and software for your project. Here are some resources you may want to check into.

- #### Sensors

  We have a number of
  [different hardware sensors](sensors.md)
  which you could use for your project; they're available for you to
  check out in lab. Let us know if you'd like to use one of them.

- #### Part suppliers

  Here are some good vendors for electronics hardware.

  * [Jameco](https://www.jameco.com/) 
  has a great selection of electronic parts.
  They are also located in Burlingame near SFO. 
  If you place an online order, parts will generally arrive the next day via US mail.
  If you have a car, 
  you can also drive there and pick up parts same day at the Will Call desk.

  * [Pololu](https://www.pololu.com/) has a wide selection of parts for robotics,
  including sensors and motor drivers.
  They also document their parts very well.

  * [Adafruit](https://www.adafruit.com/)
  Limor Fried has founded a company that caters
  to makers of all types.
  Besides building great products,
  they also provide extensive tutorials and learning guides on their web site.

  * [Sparkfun](https://www.sparkfun.com/) is another great source for hobbyist electronics.

  * Visit 
  [lab64](https://ee.stanford.edu/news/lab64), the EE department's Maker Space located in the basement of the Packard building. This workspace has a variety of tools (oscilloscope, soldering stations, 3-D printer, and more) and a store stocked with basic electronic parts such as transistors, op-amps, etc.


- #### Support code
  The directory [cs107e/src](https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/src) contains source code for the support modules from `libpi` and extra modules for `libpiextra`.

  The Raspberry Pi has on-board SPI and I2C controllers, which you can use to
  communicate with many kinds of sensors and devices that speak those protocols.
  For example, if you need to read analog input, we have an analog-to-digital
  converter, the MCP3008, which would communicate over a SPI interface with your Pi.
  You are welcome to write your own SPI or I2C driver for your device as part of your
  project. However, if 
  you would like to use an existing implementation, `libpiextra` contains the modules `i2c` and `spi` that
  you may use (make sure to cite that you used these implementations).

  `libpiextra` also contains an experimental filesystem interface
  which you can use to interact with the files on the SD card. This enables you to
  do operating-system-like things, such as run programs off the card,
  or store large data files without having to copy them over the
  bootloader every time, or save documents.


### Suggestions and inspirations

The direction you take the project is open-ended. The best project is one that that you thought of yourself and that resonates with your interests. 

It is fun to see what others have done and can help get your creative juices flowing. Check out our [project gallery](/project_gallery/) for highlights of past quarters -- what a diverse array of clever projects we have seen! 

There are makers worldwide sharing field reports about the cool things they have concocted.
Some sites to check out:
+ [Hackaday](http://hackaday.com)
+ [Adafruit blog](http://adafruit.com/blog)
+ [Instructables](http://instructables.com)
+ [Hackster](http://hackster.io)

#### Games

You have the start of a graphics and input library.
Extend your graphics library to draw additional types of objects,
and your input library to handle a game controller.

Some examples of input devices that you could use to control your game:

- A homebrew game pad made of buttons. Most game pads have
two buttons (A and B) for game control,
and 4 buttons used as a d-pad (left, right, up, down).

- Your PS/2 keyboard and/or mouse.

- Atari 2600 paddle controllers (or other paddle controllers).

- Old-school NES, SNES, or other simple gamepads with simple digital protocols.

Once you have better input, you are all set to build a game.
Build a simple game such as Pong or Tetris,
or even better a game of your own design.

Another cool thing you can do is communicate with old game cartridges, since you
can connect the pins on the cartridge to the GPIO pins on the Pi.

#### Music

We described how to generate notes on the Raspberry Pi.
With that knowledge, you can 

- Create a simple music player that allows the user 
to choose from a list of simple melodies.
Add buttons on a breadboard for control (skip, pause, play, volume?)

- Turn your keyboard into a music keyboard,
so that keys play individual notes and pressing multiple keys 
at the same time combines the notes.
Use a combination of timers and low-level keyboard access 
to enable you to record and then loop tracks.

- Take a music synthesizer such as the Korg Monotron 
and hack it to add MIDI support with one Pi,
then have a second Pi control it over MIDI.
Alternatively, build your own music synthesizer.

#### System components

Your project should be about building a working system that does something cool.

However, if you build some support infrastructure,
then you can build a more impressive system.
A good sub-project for a team member
is to build a component of the system.

#### Analog input

  Up to now, we have only read digital values using GPIO.
The Raspberry Pi, unlike a microcontroller like an Arduino,
cannot read analog values.

Many sensors are analog (photodetectors, potentiometers, temperature sensors, joysticks).

In order to read analog values, you will need to interface an analog
to digital converter (ADC) to the Raspberry Pi.  An example of an ADC
is the MCP3008, which can be controlled from the PI using the Serial
Peripheral Interface (SPI) protocol.

Write a program that reads
analog inputs to produce an interesting result, such as a screen
  visualization or sound.

#### Network (Ambitious)

Build a simple network that allows multiple raspberry pis to communicate
over a shared medium (only one Pi can transmit at a time).
One way to do this is to connect TX and RX pins to a
shared line.

It is also possible to communicate by other means.
For example, you could use sound, light, or radio waves.

Finally, you've seen in class how "digital" lines aren't purely digital.
You could design your circuit to make bits as sharp as possible to see how
fast you can transmit and receive. You might find that sending bits faster
means you'll have to send smaller packets, so optimize for overall data
  transfer, not just the bit rate.

#### GPU (Ambitious)

  The raspberry pi has a very powerful GPU.
  In fact, the GPU is much faster than the CPU.
  The GPU assembly language is similar to,
  but different,
  than the CPU assembly language.
  It is possible to download code to the GPU using a mailbox,
  and then call that code.
  Learn about the GPU and build a library for programming the GPU.

  Can you think of a computationally intensive task 
  that could be off-loaded to the GPU? At the very least, you can clear your framebuffer faster!
