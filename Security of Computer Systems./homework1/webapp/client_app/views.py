import os
import pwd

import pam
from flask import Blueprint, request, abort, url_for, redirect

blueprint = Blueprint('client', __name__)


@blueprint.route('/login', methods=['GET'])
def login():
    """Return login form."""

    return '''
     <form action="/client" method="post">
      <div class="container">
        <label for="username"><b>Username</b></label>
        <input type="text" placeholder="Enter Username" name="username" required>
    
        <label for="password"><b>Password</b></label>
        <input type="password" placeholder="Enter Password" name="password" required>
    
        <button type="submit">Login</button>
      </div>
    </form> 
    '''


@blueprint.route('/client', methods=['POST'])
def client():
    if 'username' in request.form.keys() and 'password' in request.form.keys():
        username = request.form['username']
        password = request.form['password']
        p = pam.pam()
        success = p.authenticate(username, password, service='login')

        if not success:
            return redirect(url_for('client.login'))

        result = ''
        for object_type in ['credits', 'deposits']:
            result += h1(object_type.capitalize())

            # Iterate through all files in directory.
            for obj in os.listdir(object_type):
                filepath = f'{object_type}/{obj}'
                if not is_owned_by_user(filepath, username):
                    # Only current user's files are shown.
                    continue

                with open(filepath, 'r') as f:
                    obj_id = obj[:-4]  # Take filename without extension.
                    result += h2(obj_id)
                    for line in f.readlines():
                        result += br(line)

        return result

    # Username or password not provided.
    return abort(400, 'Bad Request!')


def is_owned_by_user(filepath, username):
    return pwd.getpwuid(os.stat(filepath).st_uid).pw_name == username


def h1(string):
    return f'<h1>{string}</h1>'


def h2(string):
    return f'<h2>{string}</h2>'


def br(string):
    return f'{string}<br>'
