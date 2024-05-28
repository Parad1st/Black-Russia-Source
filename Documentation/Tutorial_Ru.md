## Туториал к лаунчеру Black Russia
# Открытие проекта через Android Studio
Заранее разархивируем скачаный исходник в удобное вам место. 

![Нажмите на Open](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/%D0%9E%D1%82%D0%BA%D1%80%D1%8B%D1%82%D1%8C%20%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82.png)

Открываем Android Studio. Там нажимаем open и выбираем папку с самим проектом.

![Выберите проект](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/%D0%92%D1%8B%D0%B1%D0%BE%D1%80%20%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B0.png)

Теперь выбираем место, куда вы разархивировали проект. На операционной системе Windows у папки с проектом будет зелёный значёк Android, если это так, то значит это то что нужно!

![Trust Project](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/Trust%20project.png)

Если появилось данное окно, нажимает Trust Project.
Проект должен был открыться. Если у вас появилась ошибка:

![Error](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/error%20dradle%20qut.png)

Если появилась данная ошибка, то просто наживаем на синий текст "Upgrade Gradle wrapper"
Теперь ваш проект должен открыться, и должны появиться папки:

![Открытый проект](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/%D0%9E%D1%82%D0%BA%D1%80%D1%8B%D1%82%D1%8B%D0%B8%CC%86%20%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82.png)

# Подключение server.json и stories.json
Файлы [server.json](https://github.com/Parad1st/BlackRussiaSource/blob/main/Json%20files/servers.json) и [stories.json](https://github.com/Parad1st/BlackRussiaSource/blob/main/Json%20files/stories.json) отвечают за работу мониторинга серверов и историй в самом лаунчере:
![Меню лаунчера](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/image%20(3).png)

Чтобы их подключить, их нужно изменить через любой текстовый редактор, и залить на облако и получить ***ПРЯМУЮ ССЫЛКУ!***
Для этого советую использовать [Dropbox](https://www.dropbox.com), так как там можно получить прямую ссылку если, вместо "0" в конце ссылки на файл написать "1"

Теперь в самом Android Studio переходим по пути app\java\com\blackrussia\launcher\other\Interface.java

![Interface.java](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/Interface%20%D1%84%D0%B0%D0%B8%CC%86%D0%BB.png)
Как мы можем видеть, в данном коде имеются ссылки ведущие до файлов [server.json](https://github.com/Parad1st/BlackRussiaSource/blob/main/Json%20files/servers.json) и [stories.json](https://github.com/Parad1st/BlackRussiaSource/blob/main/Json%20files/stories.json).
За место этих ссылок, вставляете свои. Напоминаю что нужна именно ***ПРЯМАЯ ССЫЛКА!***
Всё готово!

# Настройка кэша (файлы игры) и его подключение.
Теперь давайте скачаем нужные файлы для игры. К сожалению я не смог их добавить на гитхаб так что залил его на Google Drive. Скачать - [тык*](https://drive.google.com/file/d/1_8SRrvfS3Mv2AgY0arntJtsJVONnFlR2/view?usp=sharing)
Откройте его через WinRar или другой архиватор. Перейдите в папку SAMP и откройте файл settings.ini

![settings.ini](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/image_1_1.jpg)

Ничего не советую менять, кроме ника который будет стоять сразу после установки, и максимальное значение фпс рекомендую поставить как у меня.
Всё, сохраняем кэш с перезаписью архива:

![WinRarSave](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/image%20(11).png)
Теперь так же заливаете файл в облако. (прямая ссылка обязательна!)

Теперь как подключить кэш?
Для этого переходим по пути app\java\com\blackrussia\launcher\activity\LoaderActivity.java
И теперь меняете ссылку на свою:

![LoaderActivity.java](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/image%20(1).png)

Готово!

# Изменение названия лаунчера
Изменить название лаунчера очень просто! Для этого нужно перейти в app\res\values\strings\strings.xml и поменять значение "app_name" на своё:

![String](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/image%20(2).png)

Готово!

# Изменение иконки лаунчера
Поменять иконку лаунчера тоже довольно просто. Для этого вам нужна иконка 1060*1060 пикселей в формате .png
Теперь через проводник переходите по папкам и менете иконку на свою

BlackRussiaSource-main\app\src\main\res\drawable\xperiaicon.png

BlackRussiaSource-main\app\src\main\res\drawable\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-hdpi-v4\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-ldpi-v4\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-mdpi-v4\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-xhdpi-v4\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-xxhdpi-v4\ic_launcher.png

BlackRussiaSource-main\app\src\main\res\drawable-xxxhdpi-v4\ic_launcher.png

![ic_launcher](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/image%20(5).png)

Но это заменена только иконка самого приложения, менять дизайн самого лаунчера нужно здесь:

BlackRussiaSource-main\app\src\main\res\drawable\

По этому пути находятся все изоображения используемые лаунчером, в том чисте и иконка лаунчера.

![drawable](https://raw.githubusercontent.com/Parad1st/BlackRussiaSource/main/Documentation/Image/image%20(4).png)

Готово!
______________________________________
Пока что туториал не закончен, но он дополняется... Скоро допишу :-)
