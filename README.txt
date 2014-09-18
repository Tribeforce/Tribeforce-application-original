
CONTENTS OF THIS FILE
---------------------

 * Software Outline
 * Use case
 * Technology
 * About Drupal
 * Configuration and features
 * Installation profiles
 * Appearance
 * Developing for Drupal
 
SOFTWARE OUTLINE:
----------------------
Tribeforce Application Original is a web application that allows organisations to recruit, manage and evaluate people.


A built-in talent model is used for creating role profiles who serve as the basis for job interviews and performance appraisals. 

Managers can use a guided process of designing and performing a structured recruiting interview, logging and comparing 
answers and making a well informed decision. 

People already working in the organisation can be visualised in the application and managers can quickly add remarks on their 
performance. These remarks correspond, if the manager wished to do so, to the employee’s active role profile. 
This greatly enhances the accurancy and worth of the performance apraisals. 

USE CASE:
----------------------

TF1 is most suited for formal organisations that use role profiles as the dominant means to define what people have to do in 
their job. It fosters hiring and evaluation of people based on their skill and talents in a judgemental manner, most classic 
companies are familiar with. It has to be used with care as its intended use is providing clarity in the the job to be done, 
the ability to do it and the manner in which it has been done. When it is used in a conscious manner, it will provide clarity 
and a clear frame of the job and performance. When it is used in what we deem in an inappropriate manner, it will limit 
creativity and freedom of the individual which is proven to be detrimental on happiness  and productivity.

TECHNOLOGY:
-----------------------

The current solution is built on Drupal 7 and designed to run on a LAMP stack. A functional version is currently online. 
Access can be gained by simple request to stijn@tribeforce.com

Current status:
The software is functional in a single server environment, yet contains bugs. 
Stijn De Winter  is the contact person, reach out on stijn@tribeforce.com. 

 

ABOUT DRUPAL
------------

Drupal is an open source content management platform supporting a variety of
websites ranging from personal weblogs to large community-driven websites. For
more information, see the Drupal website at http://drupal.org/, and join the
Drupal community at http://drupal.org/community.

Legal information about Drupal:
 * Know your rights when using Drupal:
   See LICENSE.txt in the same directory as this document.
 * Learn about the Drupal trademark and logo policy:
   http://drupal.com/trademark

CONFIGURATION AND FEATURES
--------------------------

Drupal core (what you get when you download and extract a drupal-x.y.tar.gz or
drupal-x.y.zip file from http://drupal.org/project/drupal) has what you need to
get started with your website. It includes several modules (extensions that add
functionality) for common website features, such as managing content, user
accounts, image uploading, and search. Core comes with many options that allow
site-specific configuration. In addition to the core modules, there are
thousands of contributed modules (for functionality not included with Drupal
core) available for download.

More about configuration:
 * Install, upgrade, and maintain Drupal:
   See INSTALL.txt and UPGRADE.txt in the same directory as this document.
 * Learn about how to use Drupal to create your site:
   http://drupal.org/documentation
 * Download contributed modules to sites/all/modules to extend Drupal's
   functionality:
   http://drupal.org/project/modules
 * See also: "Developing for Drupal" for writing your own modules, below.

INSTALLATION PROFILES
---------------------

Installation profiles define additional steps (such as enabling modules,
defining content types, etc.) that run after the base installation provided
by core when Drupal is first installed. There are two basic installation
profiles provided with Drupal core.

Installation profiles from the Drupal community modify the installation process
to provide a website for a specific use case, such as a CMS for media
publishers, a web-based project tracking tool, or a full-fledged CRM for
non-profit organizations raising money and accepting donations. They can be
distributed as bare installation profiles or as "distributions". Distributions
include Drupal core, the installation profile, and all other required
extensions, such as contributed and custom modules, themes, and third-party
libraries. Bare installation profiles require you to download Drupal Core and
the required extensions separately; place the downloaded profile in the
/profiles directory before you start the installation process. Note that the
contents of this directory may be overwritten during updates of Drupal core;
it is advised to keep code backups or use a version control system.

Additionally, modules and themes may be placed inside subdirectories in a
specific installation profile such as profiles/your_site_profile/modules and
profiles/your_site_profile/themes respectively to restrict their usage to only
sites that were installed with that specific profile.

More about installation profiles and distributions:
* Read about the difference between installation profiles and distributions:
  http://drupal.org/node/1089736
* Download contributed installation profiles and distributions:
  http://drupal.org/project/distributions
* Develop your own installation profile or distribution:
  http://drupal.org/developing/distributions

APPEARANCE
----------

In Drupal, the appearance of your site is set by the theme (themes are
extensions that set fonts, colors, and layout). Drupal core comes with several
themes. More themes are available for download, and you can also create your own
custom theme.

More about themes:
 * Download contributed themes to sites/all/themes to modify Drupal's
   appearance:
   http://drupal.org/project/themes
 * Develop your own theme:
   http://drupal.org/documentation/theme

DEVELOPING FOR DRUPAL
---------------------

Drupal contains an extensive API that allows you to add to and modify the
functionality of your site. The API consists of "hooks", which allow modules to
react to system events and customize Drupal's behavior, and functions that
standardize common operations such as database queries and form generation. The
flexible hook architecture means that you should never need to directly modify
the files that come with Drupal core to achieve the functionality you want;
instead, functionality modifications take the form of modules.

When you need new functionality for your Drupal site, search for existing
contributed modules. If you find a module that matches except for a bug or an
additional needed feature, change the module and contribute your improvements
back to the project in the form of a "patch". Create new custom modules only
when nothing existing comes close to what you need.

More about developing:
 * Search for existing contributed modules:
   http://drupal.org/project/modules
 * Contribute a patch:
   http://drupal.org/patch/submit
 * Develop your own module:
   http://drupal.org/developing/modules
 * Follow best practices:
   http://drupal.org/best-practices
 * Refer to the API documentation:
   http://api.drupal.org/api/drupal/7
