# Caverb by Kyle Marriott
An algorithmic room reverb plugin, using a Feedback Delay Network of all-pass filters with adjustable frequency-dependent absorbency.

![image](https://user-images.githubusercontent.com/46981655/160453047-74259c6f-7afc-4e07-ad07-973a9590a579.png)


## Description
Realistic and experimental reverberation time curves are synthesised from an input signal via a tapped delay line and unitary Hadamad matrix of recursive series all-pass filters. 

![AAPF Block Diagram](https://user-images.githubusercontent.com/46981655/160452174-e9824255-9226-4c9c-ade8-d5c4a676d295.png)

The amplitude and delay of early & late reflections may be adjusted independently, thanks to energy compensated gain staging throughout the signal path.

<img src="https://user-images.githubusercontent.com/46981655/160456035-940bb325-a938-4309-a4b3-875fa52346d2.png" width="50%">


Includes 14 example presets, based on I3DL2 standard, for fast assessment of possibile effects.

Written in C++ using the ASPiK framework (http://www.aspikplugins.com/), controlled via custom vector-based GUI created in Adobe Illustrator & Figma.

Submitted coursework toward the achievement of an MSc in Audio Enginering at the University of Derby (https://www.derby.ac.uk/postgraduate/entertainment-engineering-courses/audio-engineering-msc/). 
Graded and reviewed by Bruce Wiggins of WigWare (https://www.brucewiggins.co.uk/)

## Block Diagram of Signal Processing

![Caverb_VST_blockdiagram](https://user-images.githubusercontent.com/46981655/160459413-9062e113-012e-411a-92d0-1417e1f0b0a9.png)


## Primary References
- Carpentier, T., Noisternig, M. and Warusfel, O. (2014) ‘Hybrid reverberation processor with perceptual control’, in 17th International Conference on Digital Audio Effects-DAFx-14, pp. 93–100.
- De Sena, E., Hacihabiboglu, H. and Cvetkovic, Z. (2011) ‘Scattering delay network: An interactive reverberator for computer games’, in Audio Engineering Society Conference: 41st International Conference: Audio for Games. Available at: https://www.aes.org/e-lib/browse.cfm?elib=15751
- International Standards Organization (2003) Acoustics — Measurement of sound absorption in a reverberation room. BS EN ISO 354:2003. Available at: https://www.iso.org/standard/34545.html
- Jot, J.-M. and Chaigne, A. (1991) ‘Digital Delay Networks for Designing Artificial Reverberators’, in. Audio Engineering Society Convention 90, Audio Engineering Society. Available at: http://www.aes.org/e-lib/inst/browse.cfm?elib=5663
- Poletti, M.A. (1996) ‘An assisted reverberation system for controlling apparent room absorption and volume’, in Audio Engineering Society Convention 101. Audio Engineering Society Convention 101, Audio Engineering Society. Available at: https://www.aes.org/e-lib/browse.cfm?elib=7414
- Schlecht, S.J. and Habets, E.A.P. (2019) ‘Scattering in Feedback Delay Networks’, arXiv [cs.SD]. Available at: http://arxiv.org/abs/1912.08888

## License
![image](https://chooser-beta.creativecommons.org/img/cc-by.21b728bb.svg) Caverb - an Absorbent Allpass FDN Room Reverb VST © 2020 by Kyle Marriott is licensed under Creative Commons Attribution 4.0 International.

CC BY 4.0
This license requires that reusers give credit to the creator. It allows reusers to distribute, remix, adapt, and build upon the material in any medium or format, even for commercial purposes.

To view a copy of this license, visit https://creativecommons.org/licenses/by/4.0/
