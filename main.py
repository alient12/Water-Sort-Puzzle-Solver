import numpy as np
import cv2
import os
import subprocess

image = cv2.imread("test103.png")
solve = True
src_image = np.copy(image)
# Divide image to 2 part upper side and downer side
image_list = [
    image[0 : int(image.shape[0] / 2)],
    image[int(image.shape[0] / 2) : image.shape[0]],
]

# Limit of gray color to detect the glass and
lower_gray = np.array([180])
upper_gray = np.array([188])
# lower_gray = np.array([176])
# upper_gray = np.array([190])
mask = cv2.inRange(cv2.cvtColor(image, cv2.COLOR_BGR2GRAY), lower_gray, upper_gray)
thresh = cv2.bitwise_and(
    cv2.cvtColor(image, cv2.COLOR_BGR2GRAY),
    cv2.cvtColor(image, cv2.COLOR_BGR2GRAY),
    mask=mask,
)

# Dilation to remove the noise on the edge of glass
kernel = np.ones((5, 5), np.uint8)
thresh = cv2.dilate(thresh, kernel, iterations=1)

# Create list of up and down thresh images and make Contours of them
thresh_list = [
    thresh[0 : int(thresh.shape[0] / 2)],
    thresh[int(thresh.shape[0] / 2) : thresh.shape[0]],
]
contours_list = [
    cv2.findContours(thresh_list[_], cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)[0]
    for _ in [0, 1]
]

# Detect upper and downer edge of bottles and number of bottles by area of contours
num_bottle = []
for i in [0, 1]:
    y_max = 0
    y_min = int(thresh.shape[0] / 2)
    area_max = cv2.contourArea(contours_list[i][0])
    for cnt in contours_list[i]:
        for j in range(len(cnt)):
            if cnt[j][0][1] > y_max:
                y_max = cnt[j][0][1]
            if cnt[j][0][1] < y_min:
                y_min = cnt[j][0][1]
        area = cv2.contourArea(cnt)
        if area > area_max:
            area_max = area
    thresh_list[i] = thresh_list[i][y_min - 1 : y_max]
    image_list[i] = image_list[i][y_min - 1 : y_max]
    bottles = []
    for cnt in contours_list[i]:
        if area_max * 0.9 <= cv2.contourArea(cnt) <= area_max:
            bottles.append(cnt)
    num_bottle.append(len(bottles))

print(num_bottle)

# Split bottles from each other
container_images_list = []
for i in [0, 1]:
    for j in range(num_bottle[i]):
        x = int(image_list[i].shape[1] / num_bottle[i])
        container_images_list.append(
            (
                image_list[i][:, j * x : (j + 1) * x],
                thresh_list[i][:, j * x : (j + 1) * x],
            )
        )

# remove two last bottle cause there are empty
container_images_list.pop(-1)
container_images_list.pop(-1)

color_list = []
real_color_list = []
container_list = []

# Detect color of each bottle !
for container, container_thresh in container_images_list:
    colors = []
    for i in range(4):
        x = int(container.shape[0] / 5)

        # Calculate center of mass of bottle to get color of liquid
        side_contours = cv2.findContours(
            container_thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE
        )[0]
        cx = 0
        for side_contour in side_contours:
            M = cv2.moments(side_contour)
            if M["m00"] != 0:
                cx += int(M["m10"] / M["m00"])
            else:
                cx += 0
        cx /= len(side_contours)
        w = int(cx)

        # Check if the color is new or no
        color_temp = container[(i + 1) * x][w]
        # color1 = (float(container[(i + 1) * x][w][0]) + float(container[(i + 1) * x][w][1]) + float(
        #     container[(i + 1) * x][w][2])) / 3
        # color2 = (float(container[(i + 1) * x - 20][w + 20][0]) + float(container[(i + 1) * x - 20][w + 20][1]) + float(
        #     container[(i + 1) * x - 20][w + 20][2])) / 3
        # color3 = (float(container[(i + 1) * x - 20][w - 20][0]) + float(container[(i + 1) * x - 20][w - 20][1]) + float(
        #     container[(i + 1) * x - 20][w - 20][2])) / 3
        # color = (color3 + color2 + color1) / 3
        color = (
            float(container[(i + 1) * x][w][0])
            + float(container[(i + 1) * x][w][1])
            + float(container[(i + 1) * x][w][2])
        ) / 3

        # color1 = container[(i + 1) * x][w]
        # color2 = container[(i + 1) * x - 20][w + 10]
        # color3 = container[(i + 1) * x - 20][w - 10]
        # color = ((color3 + color2 + color1) / 3).astype(np.uint8)
        # color = container[(i + 1) * x][w]
        is_color_new_flag = True
        for color_counter in color_list:
            if (
                color_counter * 0.9995 <= color <= color_counter * 1.0005
            ):  # Should Calibre for each image
                # if color_counter * 0.95 <= color <= color_counter * 1.05:  # Should Calibre for each image
                # temp_flag = 0
                # H_low = 0.5; H_High = 1.5  # Should Calibre for each image
                # S_low = 0.9; S_High = 1.5  # Should Calibre for each image
                # V_low = 0.9; V_High = 1.35  # Should Calibre for each image
                # B_low = 0.5; B_High = 1.5  # Should Calibre for each image
                # G_low = 0.5; G_High = 1.5  # Should Calibre for each image
                # R_low = 0.5; R_High = 1.5  # Should Calibre for each image
                # if cv2.cvtColor(np.reshape(color_counter, (1, 1, 3)), cv2.COLOR_BGR2HSV)[0][0][0] * H_low <= cv2.cvtColor(np.reshape(color, (1, 1, 3)), cv2.COLOR_BGR2HSV)[0][0][0] <= cv2.cvtColor(np.reshape(color_counter, (1, 1, 3)), cv2.COLOR_BGR2HSV)[0][0][0] * H_High:
                #     temp_flag += 1
                # if cv2.cvtColor(np.reshape(color_counter, (1, 1, 3)), cv2.COLOR_BGR2HSV)[0][0][1] * 0.9 <= cv2.cvtColor(np.reshape(color, (1, 1, 3)), cv2.COLOR_BGR2HSV)[0][0][1] <= cv2.cvtColor(np.reshape(color_counter, (1, 1, 3)), cv2.COLOR_BGR2HSV)[0][0][1] * S_High:
                #     temp_flag += 1
                # if cv2.cvtColor(np.reshape(color_counter, (1, 1, 3)), cv2.COLOR_BGR2HSV)[0][0][2] * 0.9 <= cv2.cvtColor(np.reshape(color, (1, 1, 3)), cv2.COLOR_BGR2HSV)[0][0][2] <= cv2.cvtColor(np.reshape(color_counter, (1, 1, 3)), cv2.COLOR_BGR2HSV)[0][0][2] * V_High:
                #     temp_flag += 1
                # if color_counter[0] * B_low <= color[0] <= color_counter[0] * B_High:
                #     temp_flag += 1
                # if color_counter[1] * G_low <= color[1] <= color_counter[1] * G_High:
                #     temp_flag += 1
                # if color_counter[2] * R_low <= color[2] <= color_counter[2] * R_High:
                #     temp_flag += 1
                # if temp_flag == 6:
                is_color_new_flag = False
                color = color_counter
                break
            else:
                continue
        if is_color_new_flag:
            # color_list.append(color.copy())
            color_list.append(color)
            real_color_list.append(color_temp.tolist())
        colors.append(color_list.index(color))
        # for check_counter in range(len(color_list)):
        #     if (color_list[check_counter] == color).all():
        #         colors.append(check_counter)
        #         break
        container[(i + 1) * x][w] = [0, 0, 0]
    colors.reverse()
    # cv2.imshow("fuck", container)
    # cv2.waitKey()
    container_list.append(colors)
print(len(color_list))

container_list.append([-1, -1, -1, -1])
container_list.append([-1, -1, -1, -1])

print(container_list)
# Color text in output
# temp = "\033[38;2;" + str(255) + ";" + str(182) + ";" + str(193) + "m"
# for aaa in container_list:
#     for i in aaa:
#         temp = "\033[38;2;" + str(real_color_list[i][2]) + ";" + str(real_color_list[i][1]) + ";" + str(real_color_list[i][0]) + "m "
#         if i == -1:
#             temp = "\033[38;2;0;0;0m "
#         print(temp + str(i) + '\033[0m', end='')
#     print()

if solve:
    command = os.getcwd() + "\Solver\main.exe"
else:
    command = os.getcwd() + "\Solver\show.exe"

temp_str = ""
temp_str += " " + str(num_bottle[0] + num_bottle[1])
for container in container_list:
    for i in container:
        temp_str += " " + str(i)
command += temp_str
print(temp_str)

# os.system(command)
output = subprocess.check_output(command)
output = output.decode("ascii")

while output.find("\n") != -1:
    temp_n = output.find("\n")
    path_str = output[0:temp_n]
    output = output[temp_n + 1 :]
    if path_str.find("[") != -1:
        ignore_next_turn_flag = False
        for i in range(len(path_str)):
            if ignore_next_turn_flag:
                ignore_next_turn_flag = False
                continue
            if (
                path_str[i] != "["
                and path_str[i] != "]"
                and path_str[i] != " "
                and path_str[i] != "\n"
                and path_str[i] != "\r"
            ):
                temp_str = path_str[i]
                if (
                    path_str[i + 1] != "["
                    and path_str[i + 1] != "]"
                    and path_str[i + 1] != " "
                    and path_str[i + 1] != "\n"
                    and path_str[i + 1] != "\r"
                ):
                    temp_str = path_str[i] + path_str[i + 1]
                    ignore_next_turn_flag = True
                temp_color = (
                    "\033[38;2;"
                    + str(real_color_list[int(temp_str)][2])
                    + ";"
                    + str(real_color_list[int(temp_str)][1])
                    + ";"
                    + str(real_color_list[int(temp_str)][0])
                    + "m"
                )
                # print(temp_color + str(path_str[i]) + '\033[0m', end='')
                print(temp_color + "■" + "\033[0m", end="")
            else:
                print(path_str[i], end="")
        print()
    else:
        print(path_str)
