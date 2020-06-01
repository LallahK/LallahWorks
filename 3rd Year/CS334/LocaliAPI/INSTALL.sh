#!/bin/sh

python3 -m venv venv
source venv/bin/activate


pip install -r requirements.txt

# An example of how the path should look:
# sed -i '' '3s/.*/from six import python_2_unicode_compatible/' /Users/sarahmeinie/group14-rw334-project-2/LocaliAPI/venv/lib/python3.7/site-packages/jet/models.py
sed -i '' '3s/.*/from six import python_2_unicode_compatible/' */venv/lib/python3.7/site-packages/jet/models.py
