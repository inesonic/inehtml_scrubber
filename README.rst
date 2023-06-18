================
inehtml_scrubber
================
A small C++ library that will scan HTML using a heuristic algorithm to remove
common nonces and other content that is found to change under normal
circumstances with each page fetch.  The generated output can be hashed to
generate a hash suitable for detecting notable changes in page content.

The library was developed for use by the SpeedSentry project.
