from random import random
from kivy.app import App
from kivy.uix.widget import Widget
from kivy.uix.button import Button
from kivy.graphics import Color, Ellipse, Line
from closest_pair import closestpair


class MyPaintWidget(Widget):

    pairs = []

    def on_touch_down(self, touch):
        if touch.x > 200 or touch.y > 100:
            self.pairs.append(((touch.x), (touch.y)))
            color = (random(), 1, 1)
            with self.canvas:
                Color(*color, mode='hsv')
                d = 30.
                Ellipse(pos=(touch.x - d / 2, touch.y - d / 2), size=(d, d))

    def clear_pairs(self):
        self.pairs = []

    def run_closest_pair(self):
        pair = closestpair(self.pairs)
        with self.canvas:
            Color((0,0,0), mode='hsv')
            d = 30
            Ellipse(pos=(pair[0][0] - d / 2, pair[0][1] - d / 2), size=(d, d))
            Ellipse(pos=(pair[1][0] - d / 2, pair[1][1] - d / 2), size=(d, d))


class MyApp(App):

    def build(self):
        parent = Widget()

        self.painter = MyPaintWidget()

        # Buttons
        clearbtn = Button(text='Clear', pos=(100, 0))
        clearbtn.bind(on_release=self.clear_canvas)
        runbtn = Button(text='Run')
        runbtn.bind(on_release=self.run_pair)

        # Add widgets
        parent.add_widget(runbtn)
        parent.add_widget(clearbtn)
        parent.add_widget(self.painter)

        return parent

    def clear_canvas(self, obj):
        self.painter.clear_pairs()
        self.painter.canvas.clear()

    def run_pair(self, obj):
        self.painter.run_closest_pair()

if __name__ == '__main__':
    MyApp().run()
