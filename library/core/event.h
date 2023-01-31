#pragma once
#include "pch.h"

namespace prncss
{
    class event
    {
    public:
        enum type
        {
            none, window_close, window_resize,
            key_down, key_up, mouse_move,
        };
    public:
        virtual type get_type() const { return type::none; }
    public:
        bool handled = false;
    };
    class event_window_close : public event
    {
    public:
        event_window_close() { }
        virtual type get_type() const override { return type::window_close; }
    };
    class event_window_resize : public event
    {
    public:
        event_window_resize(int width, int height) { m_width = width; m_height = height; }
        int get_width() const { return m_width; }
        int get_height() const { return m_height; }
        virtual type get_type() const override { return type::window_resize; }
    private:
        int m_width, m_height;
    };
    class event_key_down : public event
    {
    public:
        event_key_down(int keycode) { m_keycode = keycode; }
        int get_keycode() const { return m_keycode; }
        virtual type get_type() const override { return type::key_down; }
    private:
        int m_keycode;
    };
    class event_key_up : public event
    {
    public:
        event_key_up(int keycode) { m_keycode = keycode; }
        int get_keycode() const { return m_keycode; }
        virtual type get_type() const override { return type::key_up; }
    private:
        int m_keycode;
    };
    class event_mouse_move : public event
    {
    public:
        event_mouse_move(int x, int y, int offset_x, int offset_y) { m_x = x; m_y = y; m_offset_x = offset_x; m_offset_y = offset_y; }
        int get_x() const { return m_x; }
        int get_y() const { return m_y; }
        int get_offset_x() const { return m_offset_x; }
        int get_offset_y() const { return m_offset_y; }
        virtual type get_type() const override { return type::mouse_move; }
    private:
        int m_x, m_y, m_offset_x, m_offset_y;
    };
}