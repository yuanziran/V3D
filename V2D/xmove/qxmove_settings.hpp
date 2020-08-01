#include"../qcustomplot.h"
#include<QDialog>
#include<QLabel>
#include<QLineEdit>
#include<QFormLayout>
#include<QComboBox>
#include<QPushButton>
namespace V3D
{
    /**
     * @brief XMoveSettings 设置
    */
    class QXMoveSettings : public QDialog
	{
        Q_OBJECT
        public:
        QXMoveSettings(int timeLength,int height,int width,int frameRation,QWidget* parent=nullptr);
        int timeLength()const;
        void setTimeLength(const int length);
        int  moveHeight()const;
        void setMoveHeight(const int height);
        int  moveWidth()const;
        void setMoveWidth(const int width);
        int  moveFrameRation()const;
        void setMoveFrameRation(const int frameRation);
        QCPColorGradient colorgradient()const;
        //void setColorgradient(const QCPColorGradient& gradient);
        protected:
        void initUI();
        protected:
        QLabel*             ui_time_length_label;
        QLineEdit*          ui_time_length_data;
        QLabel*             ui_move_height_label;
        QLineEdit*          ui_move_height_data;
        QLabel*             ui_move_width_label;
        QLineEdit*          ui_move_width_data;
        QLabel*             ui_move_frame_ration_label;
        QLineEdit*          ui_move_frame_ration_data;
        QLabel*             ui_color_label;
        QComboBox*          ui_combobox_data;
        QPushButton*        ui_reject_button;
        QPushButton*        ui_accept_button;
        QFormLayout*        ui_main_layout;
	};
}