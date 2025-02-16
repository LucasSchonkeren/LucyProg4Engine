# Lucy's Prog4 engine 
By Lucas Schonkeren, 2DAE GD11(?)

## Disclaimer
I've never written a proper README in my life. For the time being this will be part credits, explanation, and documentation. This document may very well change completely and rapidly between commits.

## External Code/Software used
* [Minigin, Alex Vanden Abeele](https://github.com/avadae/minigin)


# Some documentation

(I have chosen to use the UE name Actor in place of GameObject because it's shorter)

## Philosophy

Below are some of the key design philosophies I would like to adhere to while designing this Engine.

### Everything is an Actor
Ideally, every single piece of state and behaviour within a game is encapsulated within an Actor. Actors exist within a tree, where every Actor is owned and has their lifetime controlled by a parent Actor, all the way up to a single 'root' actor, which defines a single game Scene. Every piece of state or behaviour that can be abstracted into an actor, should be. This allows for maximum uniformity and reusability across the Engine, and for precise control over the lifetime of objects.

### Actors are defined by their Components and their Children
An Actor is little more than a collection of pointers to Components and other Actors. An actor itself should not store any state or behaviour related to a game. A Component is a single, tight unit of state and behaviour. Components should ideally adhere to the Single Responsibility Principle, they should do one thing and one thing only. An actor is equally defined by its components as by its children. An actor is considered fully responsible over its children, and the state of its children inform the state of the Actor. 

Any singular 'branch' along the Actor tree, that is an Actor combined with all of its children and its children's children, should be a single, coherent, 'thing'. At the highest level, the Root Actor combined with every other Actor in the tree defines a Scene. If the state or behaviour of a single 'branch' cannot be explained singularly, consider splitting it into multiple branches with a single, common parent.

When an actor is destroyed, all of its children are destroyed as well. If a child must persist through the destruction of its parent, it must manually be moved into a different part of the tree. 

### Actors are 'solid'
An actor should be a singular, 'solid' thing, defined by its components. An Actor should be 'solid' in that its components may not be deleted or moved unless the entire Actor is. Components may freely be added throughout the lifetime of an Actor, but once added may not be deleted or moved until (and must be destroyed/moved upon) the destruction or moving of the Actor. If ever you are required to delete or move one or more components within an Actor, consider moving those components into a child Actor instead.

A reference to a Component within another Component on the same Actor may never become invalid.