# ASE_KM_Caverb
An algorithmic room reverb plugin, using a Feedback Delay Network of all-pass filters with adjustable frequency-dependent absorbency.
![image](https://user-images.githubusercontent.com/46981655/160453047-74259c6f-7afc-4e07-ad07-973a9590a579.png)


## Description
Realistic and experimental reverberation time curves are synthesised from an input signal via a tapped delay line and unitary Hadamad matrix of recursive series all-pass filters. 
![AAPF Block Diagram](https://user-images.githubusercontent.com/46981655/160452174-e9824255-9226-4c9c-ade8-d5c4a676d295.png)

The amplitude and delay of early & late reflections may be adjusted independently, thanks to energy compensated gain staging throughout the signal path.
![Energy Gain Equations](https://user-images.githubusercontent.com/46981655/160452274-e2329004-918a-4e65-8fbc-df622e390461.png)

Includes 14 example presets, based on I3DL2 standard, for fast assessment of possibile effects.

Written in C++ using the ASPiK framework (http://www.aspikplugins.com/), controlled via custom vector-based GUI created in Adobe Illustrator & Figma.

Submitted coursework toward the achievement of an MSc in Audio Enginering at the University of Derby (https://www.derby.ac.uk/postgraduate/entertainment-engineering-courses/audio-engineering-msc/). Graded and reviewed by Bruce Wiggins of WigWare (https://www.brucewiggins.co.uk/)

## Primary References
Carpentier, T., Noisternig, M. and Warusfel, O. (2014) ‘Hybrid reverberation processor with perceptual control’, in 17th International Conference on Digital Audio Effects-DAFx-14, pp. 93–100.
De Sena, E., Hacihabiboglu, H. and Cvetkovic, Z. (2011) ‘Scattering delay network: An interactive reverberator for computer games’, in Audio Engineering Society Conference: 41st International Conference: Audio for Games. Available at: https://www.aes.org/e-lib/browse.cfm?elib=15751 (Accessed: 18 January 2021).
International Standards Organization (2003) Acoustics — Measurement of sound absorption in a reverberation room. BS EN ISO 354:2003. Available at: https://www.iso.org/standard/34545.html
Jot, J.-M. and Chaigne, A. (1991) ‘Digital Delay Networks for Designing Artificial Reverberators’, in. Audio Engineering Society Convention 90, Audio Engineering Society. Available at: http://www.aes.org/e-lib/inst/browse.cfm?elib=5663
Poletti, M.A. (1996) ‘An assisted reverberation system for controlling apparent room absorption and volume’, in Audio Engineering Society Convention 101. Audio Engineering Society Convention 101, Audio Engineering Society. Available at: https://www.aes.org/e-lib/browse.cfm?elib=7414
Schlecht, S.J. and Habets, E.A.P. (2019) ‘Scattering in Feedback Delay Networks’, arXiv [cs.SD]. Available at: http://arxiv.org/abs/1912.08888
