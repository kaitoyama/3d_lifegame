import torch
from utils import flatten

def generation(first:torch.Tensor,rule:list,filter:torch.Tensor ,count:int,double_birth:bool=False):
    if not double_birth:
        under = rule[0]
        birth = rule[1]
        over = rule[2]
    else:
        under = rule[0]
        birth_1 = rule[1][0]
        birth_2 = rule[1][1]
        over = rule[2]

    end = 0
    after = first.clone()

    proc=[]

    for _ in range(count):
        before = after.clone()
        result = torch.conv3d(before, filter, padding="same")

        # Update the result tensor directly
        after = torch.where((result >= under) & (result <= over) & (before == 1), torch.tensor(1.0), before)
        if not double_birth:
            after = torch.where((result == birth) & (before == 0), torch.tensor(1.0), after)
        else:
            after = torch.where((result == birth_1) & (before == 0), torch.tensor(1.0), after)
            after = torch.where((result == birth_2) & (before == 0), torch.tensor(1.0), after)
        after = torch.where((result < under) | (result > over), torch.tensor(0.0), after)

        after_l = list(flatten.flatten(after.tolist()))
        after_l = list(map(int, after_l))
        after_l = list(map(str, after_l))
        proc.append(int("".join(after_l),2))

        if torch.sum(after) == 0:
            break
        else:
            end += 1

    return [end, proc]