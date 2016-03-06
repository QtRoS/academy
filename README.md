## Academy

Academy is Ubuntu Phone scope which allows you to find online courses from multiple sources

## Motivation

The popularity of online schools has grown over the past several years. Improvements in technology have made online schooling a more accessible option for traditional colleges and universities. Teachers and students alike are becoming more familiar how to navigate online courses, and programs continue to improve. The number of available online courses is growing constantly. To orientate in this vast diversity of courses you need a convenient search tool. Academy scope is created exactly for this purpose! With it you will be able to search relevant courses from multiple sources: 
* Coursera 
* Udemy 
* edX 
* Udacity 

Search results contain full course description, information about the instructors and explaining videos (optional). Also you can use departments, but keep in mind that some of sources contain huge amount of courses (Coursera ~1500, Udemy ~25000) which are available only via search (traffic economy).

## Installation

You can find this scope in the Ubuntu Store (more details https://uappexplorer.com/app/academy.mrqtros)

## Contributors

Feel free to fork this project or create pull requests, I will be quite happy to see them.

## License

GNU GPL v3

## Other

##### Building

To build this scope outside of QtCreator, please install phablet-tools and run click-buddy.

##### Localization

intltool is used to perform localization of the scope. Ensure whenever you add / rename files
that contain localizable strings you update "po/POTFILES.in".

When you want to add a new language to the translation catalogue:

 * Copy the .pot file from the "po" directory and rename it to e.g. "zh_CN.po", 
   where "zh_CN" is the language code for China.
 * Enter the language code in "Language:".
 * Enter the "UTF-8" as the "Content-Type:".
 * Enter the translations in the msgstr tags.

For the new translation to show up, cmake needs to be re-run to enable the recusive search
macro to delect the new translation. In QtCreator this can be done with the "Build->Run Cmake"
menu entry.

Remember! When you copy the .pot file, make sure you change the file extension to ".po".

**AND** don't forget to translate both `academy.ini.in` and `academy-settings.ini.in`.

