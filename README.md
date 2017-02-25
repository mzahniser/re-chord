# re-chord
This is a very minimal program designed for outputting PDFs of chord charts and song lyrics, including in booklet form. The format of the input files is similar to [Chordᵢᵢ](http://www.vromans.org/johan/projects/Chordii/), and you should use it or its planned [successor](https://github.com/sciurius/chordpro) instead of this if you want more functionality.

This program does have some unique features, including lining up counterpoint text with the main lyrics and chords:

![formatting example](https://cloud.githubusercontent.com/assets/11450262/23328049/f22f4402-fae6-11e6-8d12-5f5fdd35081f.png)

My other design goal was to enforce a common aesthetic in all chord charts, rather than having each song be slightly different - following the same philosophy as the LaTeX typesetting system. For example, rather than indenting with an arbitrary number of spaces, any line is either indented by a fixed amount or not indented. Chords that start before the lyrics can "overhang" into the left margin, which I thought looked better than indenting the text slightly on just those lines.

## System requirements
I've only tested this program on Linux. It uses the following libraries:
  * libcairomm-1.0-dev
  * libfontconfig1-dev

Output of some unicode characters to the PDF might not work with version 1.10 or earlier of libcairo.

## Settings
Various settings can be specified in a ".conf" configuration file. Most settings inherit a default value based on one of the other settings if you do not specify anything. For example, if you set the font size of the main text ("text-size"), all the other fonts will scale accordingly.

|key | default | explanation|
|----|----|----|
|page-width | 8.5 in | Width of the booklet page, not the output PDF's pages..|
|page-height | 11 in | If layout is 2up or booklet, there will be two pages per PDF page.|
|margin-left | 1 in | |
|margin-right | 1 in | |
|margin-top | 1 in | |
|margin-bottom | 1 in | |
|line-indent | 0.5 in | Distance that lines starting with whitespace are indented.|
|block-indent | 0.2 in | Width of whitespace when the chord downbeat is before the words.|
| |  | |
|title-font | chord-font | Song titles.|
|title-size | 1.2 * text-size | |
|title-base | 0.75 * title-size | |
|title-height | 1.2 * title-size | |
|subtitle-font | subtext-font | Typically used to name the song's author.|
|subtitle-size | .8 * subtext-size | |
|subtitle-base | .75 * subtitle-size | |
|subtitle-height | 1.2 * subtitle-size | |
| |  | |
|chord-font | Ubuntu:style=Medium | |
|chord-size | text-size | |
|chord-base | 0.75 * chord-size | |
|chord-height | 1.2 * chord-size | |
|text-font | Ubuntu:style=Regular | Song lyrics.|
|text-size | 12 pt | |
|text-base | 0.75 * text-size | |
|text-height | 1.2 * text-size | |
|subtext-font | Ubuntu:style=Italic | Comments, etc.|
|subtext-size | text-size | |
|subtext-base | 0.75 * subtext-size | |
|subtext-height | 1.2 * subtext-size | |
| |  | |
|number-font | subtitle-font | Page numbers.|
|number-size | subtitle-size | |
|number-base | 0.75 * number-size | |
|number-height | 1.2 * number-size | |
| |  | |
|index-font | text-font | Table of contents entries.|
|index-size | text-size | |
|index-base | text-base | |
|index-height | text-height | |
| |  | |
|line-gap | text-size * .25 | The space to insert between non-empty lines.|
|stanza-gap | text-size * 2 | The height of an empty line.|
|title-gap | stanza-gap | The gap between the title block and the text.|
| |  | |
|index-location | none | none / front / back|
|layout | single | single / 2up / booklet|
