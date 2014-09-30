(defun main ()
  (setq args (nthcdr 5 command-line-args))
  (setq options (parse-args args))

  (setq words-file (or (cdr (assoc 'words options))
                       (expand-file-name "../words.txt" (__dirname))))

  (setq phone-file (or (cdr (assoc 'phone options))
                       (expand-file-name "../phone.txt" (__dirname))))

  (setq words-list (build-word-list words-file))
  (setq digit-to-letters-list (build-digit-to-letters-list phone-file))
  (setq letter-to-digit-map (build-letter-to-digit-map digit-to-letters-list))
  (setq digits-to-words-map (build-digits-to-words-map words-list letter-to-digit-map))

  (setq
   process-line
   (if (cdr (assoc 'reverse options))
       (lambda (line) (translate-sentence-to-digits line letter-to-digit-map))
     (lambda (line) (translate-digits-to-sentences line digits-to-words-map))))

  (let (line)
    (while (setq line (read-from-minibuffer ""))
      (princ (funcall process-line line))
      (princ "\n"))))

(defun __dirname () (file-name-directory load-file-name))

(defun parse-args (args)
  (let (arg (options nil))
    (while (> (length args) 0)
      (setq arg (car args))
      (setq args (cdr args))
      (cond
       ((or (string= arg "-p") (string= arg "--phone-file"))
        (setq arg (car args))
        (setq args (cdr args))
        (setq options (append `((phone . ,arg)) options)))
       ((or (string= arg "-w") (string= arg "--words-file"))
        (setq arg (car args))
        (setq args (cdr args))
        (setq options (append `((words . ,arg)) options)))
       ((or (string= arg "-r") (string= arg "--reverse"))
        (setq options (append `((reverse . t)) options)))
       (t (print (concat "Unknow flag " arg)))))
    options))

(defun read-lines (filePath)
  "Return a list of lines in FILEPATH."
  (with-temp-buffer
    (insert-file-contents filePath)
    (split-string
     (buffer-string) "\n" t)))

(defun build-word-list (words-file) (read-lines words-file))

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

(defun build-digit-to-letters-list (phone-file)
  (mapcar
     (lambda (line)
       (split-string line "," t))
     (read-lines phone-file)))

(defun build-letter-to-digit-map (digit-to-letters)
  (let ((table (make-hash-table :test 'equal)))
    (mapc
     (lambda (item)
       (let ((digit (car item))
             (letters (nth 1 item)))
         (mapc
          (lambda (letter) (puthash letter digit table))
          (string-to-list letters))))
     digit-to-letters)
    table))

(defun word-to-digits (word letter-to-digit-map)
  (mapconcat
   (lambda (letter)
     (gethash letter letter-to-digit-map))
   (string-to-list (upcase word)) ""))

(defun translate-sentence-to-digits (sentence letter-to-digit-map)
  (word-to-digits sentence letter-to-digit-map))

(defun build-digits-to-words-map (words letter-to-digit-map)
  (let ((table (make-hash-table :test 'equal)))
    (mapc
     (lambda (word)
       (let ((digits (word-to-digits word letter-to-digit-map)))
         (puthash digits (append (gethash digits table) (list word)) table)))
     words)
    table))

;;; string -> ((string))
(defun encode-digits (digits digits-to-words-map)
  (let
      ((len (length digits)))
    (if (= len 0)
        (list (list))
      (apply
       'append
       (mapcar
        (lambda (split)
          (let* ((tuple (string-partition digits split))
                 (head (car tuple))
                 (rest (cdr tuple))
                 (prefixes (gethash head digits-to-words-map)))
            (apply
             'append
             (mapcar
              (lambda (first-word)
                (mapcar
                 (lambda (following-words)
                   (append (list first-word) following-words))
                 (encode-digits rest digits-to-words-map)))
              prefixes))))
        (number-sequence 1 len))))))

(defun translate-digits-to-sentences (digits digits-to-words-map)
  (mapconcat
   (lambda (x)
     (concat digits ": " (mapconcat 'identity x " ")))
   (encode-digits digits digits-to-words-map) "\n"))

(main)
