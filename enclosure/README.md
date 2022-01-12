Note: it is not suggested to create an enclosure until a stripboard prototype is tried and tested. A step-by-step guide can be found in the [pm25_neopixel_esp32 directory](TODO).

1. Plug the prototype into the power bank using a USB cable, and confirm that the NeoPixel LEDs are at least partially lit up.
2. Position the power bank atop the PMS5003.
3. Measure the width, height, and depth of the prototype + power bank, ideally with a digital caliper for added precision.
4. Add 3 mm to each dimension to ensure adequate space is available and note these dimensions for the inside of a 3d-printed box. Add another 2 mm for the dimensions of the outside of the box.
5. [Design a box in Fusion 360](https://www.youtube.com/watch?v=HDJ2g19SlCI)
6. [Make one side meshed to allow air to flow into the Plantower](https://www.youtube.com/watch?v=Kj6h6ARtITE&t=176s). This side will be faced towards the cyclist to reduce distortion induced by the PMS5003's fan.
7. 3D Print this box, and ensure that your powered device fits inside of it with the power bank closest to the opening.
8. If the device fits in the box, take measurements for your mount. The mount I printed in this project has three points of support: one at the middle of the stem, and two on the handlebar. A gap needs to be left for the stem's handlebar mount. Take the following measurements:
   * Length and width of the stem's handlebar mount
   * Height difference between the top of the handlebar mount and the tops of both stem and the handlebar at its lowest point (if your handlebar has curvature). Note down whichever height difference is greater.
   * Width of the stem at the approximate point of support
   * Width of the handlebar at the approximate points of support
9.  Return to your 3D Modelling software and produce the base with the following considerations:
    * To fit the boxed device securely, the base will need a .3 mm larger width and length than that of the outer dimensions of the box
    * Vault the edges of the supports slightly for added stability
    * Create a hole within each of the three supports to fit cable ties to create three supports in two axes of rotation
10. Once the base is fully printed, it can be mounted on the bicycle with the cable ties.
11. Carefully place the box upside-down on the mount, preventing the contents from falling out
12. Rubber bands were additionally used during test rides to secure the mount to the base.
