***************************************************************
	Program:			ClothSimulation		
							
	Author:				Ben Howenstein (C19)	
	Last modified:		3/25/13			
									
	Description:					
							
	This program demonstrates soft-body collision	
	detection and response.  The cloth is modeled as
	a mass-spring-damper particle system, using 	
	a Symplectic Euler integration to compute	
	the position of each particle with respect to	
	simulation time and the sum of forces acting on it. 
							
	Costraint relaxation is applied between pairs of
	of connected particles to increase numerical	
	stability for relatively large spring stiffness	
	coefficients.					
							
	The combination of cheap forward integration	
	and constraint relaxation with few iterations per 
	simulation timestep allows the simulation to 	
	achieve greater numerical stability and run-time 
	efficiency than using either apporach in isolation.
*****************************************************************

~~~~	Controls	~~~~

Camera:

* Mouse
	
	left-button (click):		engage/disengage mouse and keyboard controls
	right-button (hold down):	W/S keys move camera up/down

* Keyboard

	W:		move camera forward/up
	A:		move camera left
	S:		move camera backward/down
	D:		move camera right

Simulation:

* Keyboard

	1:		reset simulation with vertical cloth
	2:		reset simulation with horizontal cloth
	B:		spawn ball
	U:		detach cloth
	[:		move air jets up
	]:		move air jets down
	0:		toggle air jets on/off
	8:		toggle ball gravity on/off
	K:		select shear springs
	L:		select stretch springs
	+/-:	increase/decrease spring stiffness
