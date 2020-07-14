from flask import Flask, request

app = Flask(__name__)

@app.route("/get_phase_info", methods=["GET"])
def handle_phase_req():
    print(request.args)
    return "SUCCESS"

@app.route("/post_result", methods=["POST"])
def handle_results():
    data = request.form

    if 'exploded' not in data:
        print('ERROR: Invalid post result')
        return "FAILURE"

    if data['exploded'] == '0':
        print('Handling defuse: {}'.format(data))
        return handle_defuse(data)
    else:
        print('Handling explode: {}'.format(data))
        handle_explode(data)
        return 'SUCCESS'

def handle_defuse(data):
    return 'SUCCESS'

def handle_explode(data):
    pass
