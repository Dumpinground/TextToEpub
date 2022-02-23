import asyncio
import json
import sys
import time
from pathlib import Path

from httpx import AsyncClient
from tenacity import retry, stop_after_attempt

curFileDir = Path(sys.argv[0]).parent  # 当前文件路径

try:
    with open(curFileDir / "config.json", "r", encoding="utf-8") as f:
        config = json.load(f)
except:
    print("载入配置文件出错")
    exit(0)

# print(config)
cookie: dict = config["cookie"]
csrf = config["csrf"]

header: dict = config["header"]
header["cookie"] = cookie
task_map = config["task_map"]
sleep_time = float(config["sleep_time"])


@retry(stop=stop_after_attempt(5), retry_error_callback=lambda retry_state: None)
async def get_reward(client: AsyncClient, data, reward_name):
    """
    领取奖励
    :param client:
    :param data: post的data
    :param reward_name: 奖励的名字
    :return: None
    """
    url = "https://api.bilibili.com/x/activity/mission/task/reward/receive"
    try:
        rep = await client.post(url, data=data)
        res = rep.json()
    except:
        print("请求错误")
        return
    # print(f"res: {res}")
    if res["code"] == 0:
        print(f"{'》' * 10}[[领取成功]]{reward_name} : 领取成功{'《' * 10}")
    else:
        print(f'{"》" * 10}[[领取失败]]{reward_name}: {res["code"]}--{res["message"]}{"《" * 10}')
        raise Exception


async def get_reward_info(client: AsyncClient, taskid):
    """
    获取奖励的参数
    :param client:
    :param taskid:
    :return: 返回None的话就是不能领取的奖励
    """
    # print(taskid)
    url = "https://api.bilibili.com/x/activity/mission/single_task"
    params = {
        "csrf": csrf,
        "id": taskid
    }
    try:
        response = await client.get(url, params=params)
        res = response.json()
    except:
        print("请求错误")
        return None
    # print(res)
    if res["code"] != 0:
        print("状态码错误")
        return None
    reward_info = f'[{res["data"]["task_info"]["reward_info"]["reward_name"]}]  >>>剩余数量:[{res["data"]["task_info"]["reward_period_stock_num"]}]<<<'
    if res["data"]["task_info"]["receive_status"] == 3:
        print(f'已领取过: {reward_info}')
        task_map[taskid] = False
        return None
    elif res["data"]["task_info"]["reward_period_stock_num"] == 0:
        print(f'已领完: {reward_info}')
        # task_map[taskid] = False
        return None
    elif res["data"]["task_info"]["receive_status"] == 0:
        print(f'无法领取: {reward_info}')
        # task_map[taskid] = False
        return None
    print(f"尝试领取: {reward_info}")
    data = {
        "csrf": csrf,
        "act_id": res["data"]["task_info"]["group_list"][0]["act_id"],
        "task_id": res["data"]["task_info"]["group_list"][0]["task_id"],
        "group_id": res["data"]["task_info"]["group_list"][0]["group_id"],
        "receive_id": res["data"]["task_info"]["receive_id"],
        "receive_from": "missionLandingPage"
    }
    await get_reward(client, data, res["data"]["task_info"]["reward_info"]["reward_name"])


async def do_all_task(client):
    start = time.time()
    tasks = []
    for k, v in task_map.items():
        if v:
            a_task = asyncio.create_task(get_reward_info(client, k))
            tasks.append(a_task)
    # print(tasks)
    await asyncio.gather(*tasks)
    return time.time() - start


async def main():
    async with AsyncClient(headers=header) as client:
        while True:
            print("start")
            use_time = await do_all_task(client)
            print(f"用时:{round(use_time, 3)}s")
            print(f"下一次循环将在{sleep_time}s后")
            print("#" * 55)
            if use_time >= sleep_time:
                continue
            await asyncio.sleep(sleep_time - use_time)


if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())
    print("end2")
