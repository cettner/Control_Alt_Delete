# Aether Forge

Hello and welcome to the source code repository for my game, Aether Forge! ..Which currently operates under the repository Control_Alt_Delete.  I've worked on for a little over 4 years now, is solo developed, and my first attempt at utilizing the Unreal Engine.  The Project is coded almost entirely in c++, utilizing UE's visual scripting language blueprints as modular data-only graphs for default data manipulation and asset managment.  It's from this framework I've been able to build a scalable, modular codebase that allows easy tuning and addition to the project. 

At its core, Aether Forge is an ambitious multiplayer RTS / FPS Hybrid with RPG Elements.  Players will play in teams either as an all seeing oracle (RTS) or one of potentially multiple commander (FPS) units.  The core ideology behind Aether Forge is that while both play types can ultimately do each others job individually, teamwork and cooporation will yield the highest reward.

## **PreGame Systems**
### **Main Menu**
The Main Menu offers a flexible set of offline widgets implemented in C++ amd designed in UMG giving the player conroller over most local settings.  It's also responisble for finding available online sessions via the OnlineSubsystem.  Currently supports Steam and LAN Connection Types. 

#### **Server Hosting**
Servers may be hosted as private or public, both are searchable over the session interface, but private games have an embedded password specified by the host.

#### **Server Joining**
Lists the available hosted servers, 

#### Future Features
1. Options Menu
2. Epic Games Session Support
3. Join Private Games Via Password

### **Lobby System**
Upon Joining the Session via the Online Subsystem Session the player is able to access a game agnostic lobby. This lobby was written for reuse for any lobby based online game and not actually tied to .  The server transition between the lobby and the game uses non-seamless travel, and stores server data in the game instance.  The recieving level needs to inherit a few top level architectural classes to ensure proper setup.  

#### Lobby Slots
Each Slot contains embedded data that gets associated with the player's unique net ID, by default this is just the associated team ID.  In the Case of the Aether Forge, the slot is overridden to designate the players RTS or FPS status in the game.

#### Future Features


## **Game Systems**
### **RTS Systems**
#### **Camera**
The RTS Camera provides a strategic view of the map, providing line of sight and awareness for the rest of the team.  It provides basic pan and zoom functionality with the keyboard.

Future Features
1. Camera Rotation 
2. Camera Edge Scrolling with the mouse.

#### **Selection**
The RTS Player is able to select units via the mouse in one of three formats, this selection has been simplified into a simple component, with multiple selection methods.
1. Single Selection
2. Multi Unit Select : Double clicking a unit will select all units of the same type on the screen
3. Box Group Selection 

##### **Selection Pane**
Unit selections are captured in the unit selection pane, which provides quick access to currently selectable units.
<img src ="./Docs/Gifs/RTSSelection.gif" alt="SkillGif" height="420">

### **FPS Systems**

#### **Ability System**
Any action performed with a weapon in the game is considered an ability, abilities can have a blend aggressive, defensive, or even logistical benefits in nature.  Depending on the equipped weapon many abilities may be executed independently of each other. 

#### **Weapon System**
All Commanders are instatiated with several weapons that each contain a small number of default abilities.  The Commander may spend talent points over the course of the game to aquire to grow additional abilities or empower/extend existing ones.  Weapons may be switched between at any time, but require a small delay before an ability may be used when being equipped/unequipped. 

<img src ="./Docs/Gifs/FPSWeaponSwitch.gif" alt="FPSWeaponSwitchGif" height="420">

##### **Mage Hands**
The "mage" playstyle / weapon type involves casting various spells and abilties in either the right, left or both hands. Each of these abilites pull from the mana pool on use.

<img src ="./Docs/Gifs/FPSFireball.gif" alt="FireballGif" height="420">

##### **BroadSword**
The Broadsword serves as the Melee Weapon Archtype, allowing for damage to be dealt at close range.  Given its lack of range, basic melee attacks do not consume mana on use.


#### **Experiance System**

### **Shared Systems**
#### **Upgrade System**
Both playtype utilizes skill trees as means of progressing either themselves individually or the team globally.  The primary goal of the codebase is to make adding upgrades to the game as simple as possible while being able to effect a variety of object. In order to achieve this, an upgradable interface was contructed, allowing modification of gameplay objects in an easy and controllable manner.  One need only create a single class, implement what the upgrade does, and specify a target class or classes for the upgrade to effect.  Even the UI and associated tooltip are generalized to make additions to the game as simple as possible.   
<img src ="./Docs/Gifs/FPSSkillTree.gif" alt="SkillGif" height="420">

#### **Resource System**
Most values in the game, be it health, mana, crystal, are all tied to the same generic resource system, this yields a great deal of flexiblity when expending or gathing resources.  

##### **Discrete Resource**
Discrete Resources are defined as resources that have a defined maximum and minimum values, examples of this include health and mana, which share no dependence on one another on thier aquisistion.  

##### **Weighted Resource**
Conversely a weighted resource is a resource that share a pooled maximum value with other weighted resource.  For example, a single character may be able to support a maximum weight of 10, if a given resource has a weight value is 2 and resource B has a weight value of 5, the character could carry:
 1. 5 units of resource A or 
 2. 2 units of resource B or 
 3. 2 units of resource A and 1 unit of resource B  
 4. Other permutations as long as the total weight value of all aquired resources is under 10


#### **Resource Types**
Currently the only Implemented Resource Type is the Blue Aether resource as a result it's weight is 1. 

##### **Collection Methods**

##### **FPS**
Currently the Commanders siphon resources via the "Mine" spell of the "mage hands" weapon, weight capacity is indicated by the resource weight bar on the left when new resources are added to thier supply.  These resources may be deposited at team structures to be made available to the team / RTS supply to be manufactured into units, structures, or upgrades.
<img src ="./Docs/Gifs/FPSMining.gif" alt="FPSMiningGif" height="420">

##### **RTS**
The RTS Player can aquire resources through the commanders donations, but can also use thier own builder units to aquire resources, builders will fetch resources of a specified type and deliver them to the nearest available droppoint until the resource is no longer available in the immediate area.
<img src ="./Docs/Gifs/RTSMining.gif" alt="RTSMiningGif" height="420">