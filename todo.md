# Short Term Goal - add enough features to be able to test any conjecture

- Initialization (DONE)
 - Finish `parseFilters()` (DONE)
- Initial Testing with PFGW (In Progress)
 - Port from Java Program (In Progress)
 - Each PFGW instance should have own result file
- Sieving and Testing
 - For each block of 10,000:
  - Start with initial sieving up to a predetermined point
  - GPU will sieve (sr2sieve?) while CPU tests
  - Multiple LLR2 instances used
  - CERT files saved and compressed but not certified

# Long Term Goal - add distributed computing features

- GitHub as file storage
 - Automatically upload CERT files to a pull request in a GitHub repo
 - These will be periodically downloaded by other users
  - Invalid tests will be redone by that user
 - Each block will be of a certain size (10,000?) for a single k
  - Size can be tweaked with global settings in the GitHub repo
- A separate program can automatically analyze and update tables that will be used in a website on GitHub Pages
- Clients can customize which k they want to run

# Non-Code Related Things
- finish first version of website
- use list of primes to find remaining k's
- use PFGW to verify every prime discovered by the old iteration of the project
