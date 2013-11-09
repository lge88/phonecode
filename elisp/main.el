
(defun read-lines (filePath)
  "Return a list of lines in FILEPATH."
  (with-temp-buffer
    (insert-file-contents filePath)
    (split-string
     (buffer-string) "\n" t)) )

(defun write-string-to-file (str file)
  (with-temp-file file
    (insert str)))

(defun string-partition (str stop)
  (let*
      ((len (length str))
       (n (cond ((> stop len) len)
                ((< stop (- len)) (- len))
                (t stop))))
    (cons
     (substring str 0 n)
     (substring str n len))))

(setq linux-words (read-lines "../words.txt"))

(setq phone-mnem
      '(
        ("2" . "ABC")
        ("3" . "DEF")
        ("4" . "GHI")
        ("5" . "JKL")
        ("6" . "MNO")
        ("7" . "PQRS")
        ("8" . "TUV")
        ("9" . "WXYZ")))

(setq
 phone-char-code
 (let
     ((table (make-hash-table)))
   (mapc
    (lambda (item)
      (let
          ((num (car item))
           (str (cdr item)))
        (mapc
         (lambda (c)
           (puthash c num table))
         (string-to-list str))))
    phone-mnem)
   table))

(defun word-to-phone-code (word)
  (mapconcat
   (lambda (c)
     (gethash c phone-char-code))
   (string-to-list (upcase word)) ""))

(setq
 linux-words-for-num
 (let
     ((table (make-hash-table :test 'equal)))
   (mapc
    (lambda (w)
      (let
          ((c (word-to-phone-code w)))
        (puthash c (append (gethash c table) (list w)) table)))
    linux-words)
   table))

;;; string -> ((word))
(defun encode-phone-number (num)
  (let
      ((len (length num)))
    (if (= len 0)
        (list (list))
      (apply
       'append
       (mapcar
        (lambda (split)
          (let* ((tuple (string-partition num split))
                 (head (car tuple))
                 (rest (cdr tuple))
                 (word-list
                  (gethash head linux-words-for-num)))
            (apply
             'append
             (mapcar
              (lambda (w)
                (mapcar
                 (lambda (l)
                   (append (list w) l))
                 (encode-phone-number rest)))
              word-list))))
        (number-sequence 1 len))))))

(defun translate-phone-number (num)
  (mapconcat
   (lambda (x)
     (mapconcat 'identity x " "))
   (encode-phone-number num) "\n"))

(write-string-to-file
 (mapconcat
  (lambda (num)
    (concat "\n" num ":\n" (translate-phone-number num)))
  (read-lines "../input.txt") "\n")
 "output.txt")
