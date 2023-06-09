#include <sys/ioctl.h>
#include <unistd.h>

#include <stdio.h>

class view
{
    private:
        int pix_x;
        int pix_y;
    public:
        view(int in_pix_x, int in_pix_y) : pix_x(in_pix_x), pix_y(in_pix_y)
        {}

        int get_pix_x()
        {
            return pix_x;
        }

        int get_pix_y()
        {
            return pix_y;
        }

        virtual void get_win_size(int* x_size_ptr, int* y_size_ptr) = 0;

        virtual bool is_win_resize() = 0;

        virtual void clear_win() = 0;

        virtual void draw(int x, int y, char a) = 0;
};

class t_view : public view
{
    private:
        int x_size;
        int y_size;
        int pr_x_size = -1;
        int pr_y_size = -1;
    public:
        t_view(int in_pix_x, int in_pix_y) : view(in_pix_x, in_pix_y)
        {}

        void get_win_size(int* x_size_ptr, int* y_size_ptr)
        {
            struct winsize argp;
            ioctl(1, TIOCGWINSZ, &argp);
            *x_size_ptr = (int) argp.ws_col;
            *y_size_ptr = (int) argp.ws_row;
        }

        bool is_win_resize()
        {
            get_win_size(&x_size, &y_size);
            if (pr_x_size != x_size || pr_y_size != y_size)
            {
                pr_x_size = x_size;
                pr_y_size = y_size;
                return 1;
            }
            return 0;
        }

        void clear_win()
        {
            for (int i = 1; i <= x_size; i++)
                for (int j = 1; j <= y_size; j++)
                {
                    printf("\e[%d;%d;H", j, i);
                    printf("%c", ' ');
                    printf("\e[H");
                }
        }


        void draw(int x, int y, char a)
        {
            for (int i = (x - 1) * (x_size / get_pix_x()) + 1; i <= x * (x_size / get_pix_x()); i++)
                for (int j = (y - 1) * (y_size / get_pix_y()) + 1; j <= y * (y_size / get_pix_y()); j++)
                {
                    printf("\e[%d;%d;H", j, i);
                    printf("%c", a);
                    printf("\e[H");
                }
        }
};
