from PIL import Image
import PIL.ImageDraw
from PIL.ImageDraw import ImageDraw as ImageDrawType

from scripts.pyLib import EffectData

# some of this file was gpted, though i changed a decent chunk to make it work the way i wanted


def toColor(li):
    return (int(li[0] * 255), int(li[1] * 255), int(li[2] * 255))


# returns "black" or "white" depend on what contrasts the given color the most
def getTextColor(rgb):
    r, g, b = rgb
    brightness = (r * 299 + g * 587 + b * 114) / 1000
    return "black" if brightness > 128 else "white"


def genEffectMap(data: list[dict]):
    s = 5
    size = (750 * s, 750 * s)
    scaler = 100 * s
    img = Image.new("RGB", size, "white")
    draw: ImageDrawType = PIL.ImageDraw.Draw(img)
    zero = (size[0] / 2, size[1] / 2)
    # for effect in data:
    #     print(effect["Name"])
    #     mag = effect["mixMagnitude"]
    #     pos = effect["mixDir"].copy()
    #     pos[0] = pos[0] * mag * scaler + (size[0] / 2)
    #     pos[1] = pos[1] * mag * scaler + (size[1] / 2)
    #     # print(pos)

    #     draw.line((zero,pos),fill="red",width=s)
    draw.circle(zero, s * 5, "white", "black", width=s * 3)
    draw.text((zero[0], zero[1] - 7 * s), "Center", 1, font_size=s * 10, anchor="mb")
    # print("\ndid the lines\n")
    for effect in data:
        print(effect["Name"])
        mag = effect["mixMagnitude"]
        pos = effect["mixDir"]
        pos[0] = pos[0] * mag * scaler + (size[0] / 2)
        pos[1] = pos[1] * mag * scaler + (size[1] / 2)
        # print(pos)

        # draw.line((zero,pos),fill="red",width=s)
        draw.circle(
            pos,
            effect["radius"] * scaler,
            outline=1 * s,
            width=2 * s,
            fill=toColor(effect["productColor"]),
        )
        # pos[0] -= draw.textlength(effect["Name"]) / 2
        # pos[1] -= 5
        si = 10 * s
        while effect["radius"] * scaler * 1.8 < draw.textlength(
            effect["Name"], font_size=si
        ):
            si -= 1

        draw.text(
            pos,
            effect["Name"],
            fill=getTextColor(toColor(effect["productColor"])),
            anchor="mm",
            font_size=si,
        )

    img.save("./extrapolatedData/effectMap.png")


if __name__ == "__main__":

    print("generating effect map img")
    genEffectMap(EffectData)
