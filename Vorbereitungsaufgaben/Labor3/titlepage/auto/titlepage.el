(TeX-add-style-hook
 "titlepage"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "12pt")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("inputenc" "utf8x") ("todonotes" "colorinlistoftodos") ("babel" "ngerman")))
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art12"
    "inputenc"
    "amsmath"
    "graphicx"
    "float"
    "todonotes"
    "xcolor"
    "babel"
    "datetime"
    "fontspec"
    "tgheros")
   (TeX-add-symbols
    "HRule")
   (LaTeX-add-xcolor-definecolors
    "hsblue"))
 :latex)

