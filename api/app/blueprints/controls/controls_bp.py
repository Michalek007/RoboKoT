from flask import request, url_for, redirect, render_template, jsonify, current_app

from app.blueprints import BlueprintSingleton
from app.blueprints.controls.modules import Action


class ControlsBp(BlueprintSingleton):
    """ Implementation of controls views.
        Attributes:
            action: current action value (enum Action type)
    """
    action = 0  # Action.Wait

    # views
    def get_action(self):
        return jsonify(action=self.action), 200  # + 1

    def update_action(self, value: int):
        self.action = value
        return jsonify(message='Action updated successfully. '), 202

    # gui views
    def controls(self):
        return render_template('controls/controls.html')
