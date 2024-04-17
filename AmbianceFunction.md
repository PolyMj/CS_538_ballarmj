# 
# General Structure / Concept

First, a raytracing simulation will be conducted on a given model. This simulation will shine light on the object from all directions, and check: 
 * How many rays bounce off of the model.
 * How their color has changed since orignally being casted.
 * What direction they're currently going.
 * What their original cast direction was.

This data will then be averaged out to a few (10-20) data points around the model. Because this data is based on light from all directions, rather than from specific sources, the goal is to be able to interpolate between this data to calculate ambient lighting given any arbitrary lighting conditions. 

At the beginning of each rendering call, there will be a lighting pass where each function will be defined using:
 * Original light cast direction in combination with each light source's direction. 
 * Light color/direction output data. 

When you need to draw something to the screen that will have ambient lighting, you'll pass the point of interest to a function that will use the vector between the POI and the model's center to interpolate between these data points and estimate what influence the model's ambient lighting will have on the point. 

#
# Current Issues / Concerns
### Lack of Raytracer
 * ***Problem:***
   * I don't even have a raytracer to being with so why am I trying to solve this problem now? I've lost sleep over this because I really want to try it but I need a lot of more important stuff before that can happen. 
   * tldr; my brain hurts and I am a fool 
 * ***Solution:***
   * Stop being foolish

### Occlusion
 * ***Problem:*** 
   * If some object is blocking light from reaching the model, the calculations will be less accurate.
   * Same concept applies when an object is between the POI and the model. 
   * Because there will be minimal raycasting to check occlusion, it's possible to get a "banding" effect, where the amount of ambient lighting changes sharply, rather than forming a proper gradient. 
 * ***Solutions:*** 
   * Do a small number of ray casts towards the most relevant data points of the model to determine which (if any) data points will actually influence the POI's lighting. 
   * Some of these rays may be casted a small distance away from the original POI, which should help interpolate between the lighting recieved by the POI and other nearby points. 